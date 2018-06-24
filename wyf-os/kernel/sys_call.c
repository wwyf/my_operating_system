#include <sys_call.h>
#include <global.h>
#include <proc/process.h>
#include <common/common.h>
#include <proc/schedule.h>
#include <message.h>




/*****************************************************************************
 *				  ldt_seg_linear
 *****************************************************************************/
/**
 * <Ring 0~1> Calculate the linear address of a certain segment of a given
 * proc.
 * 
 * @param p   Whose (the proc ptr).
 * @param idx Which (one proc has more than one segments).
 * 
 * @return  The required linear address.
 *****************************************************************************/
PRIVATE int ldt_seg_linear(proc_task_struct_t * p, int idx)
{
    /* 由于目前的进程都是内核进程，因此在同一个地址空间中，不需要转换，段地址返回0即可 */
    return 0;
	// struct descriptor * d = &p->ldts[idx];

	// return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

/**
 * @brief 由进程获取pid
 * 
 * @param p 
 * @return PRIVATE proc2pid 
 */
PRIVATE int proc2pid(proc_task_struct_t * p){
    return p->pid;
}


/**
 * <Ring 0~1> Virtual addr --> Linear addr.
 * 
 * @param pid  PID of the proc whose address is to be calculated.
 * @param va   Virtual address.
 * 
 * @return The linear address for the given virtual address.
 **************************************************************/
PRIVATE void* va2la(int pid, void* va)
{
	proc_task_struct_t * p = &g_pcb_table[pid];

	uint32_t seg_base = ldt_seg_linear(p, INDEX_LDT_RW);
	uint32_t la = seg_base + (uint32_t)va;

	if (pid < _PROC_NUM) {
		assert(la == (uint32_t)va);
	}

	return (void*)la;
}



/**
 * @brief 系统调用统一入口
 * 
 * @param regs 
 * @return PUBLIC sys_call 
 */
PUBLIC void sys_call(proc_regs_t * regs){
    int v = regs->eax;
	int ret = 0;
    switch (v){
        case TASK_SYS:{
            ret = sys_sendrec(regs->ebx, regs->ecx, (message_t*)regs->edx, (proc_task_struct_t*)regs->esi);
            break;
        }
        default:{
            com_printk("in default sys call!");
            break;
        }
    }
	/* 设置系统调用返回值 */
	regs->eax = ret;
}



/*****************************************************************************
 *                                deadlock
 *****************************************************************************/
/**
 * <Ring 0> Check whether it is safe to send a message from src to dest.
 * The routine will detect if the messaging graph contains a cycle. For
 * instance, if we have procs trying to send messages like this:
 * A -> B -> C -> A, then a deadlock occurs, because all of them will
 * wait forever. If no cycles detected, it is considered as safe.
 * 
 * @param src   Who wants to send message.
 * @param dest  To whom the message is sent.
 * 
 * @return Zero if success.
 *****************************************************************************/
PRIVATE int deadlock(int src, int dest)
{
	proc_task_struct_t * p = g_pcb_table + dest;
	// while (1) {
	// 	if (p->p_flags & SENDING) {
	// 		if (p->p_sendto == src) {
	// 			/* print the chain */
	// 			p = proc_table + dest;
	// 			printl("=_=%s", p->name);
	// 			do {
	// 				assert(p->p_msg);
	// 				p = proc_table + p->p_sendto;
	// 				printl("->%s", p->name);
	// 			} while (p != proc_table + src);
	// 			printl("=_=");

	// 			return 1;
	// 		}
	// 		p = proc_table + p->p_sendto;
	// 	}
	// 	else {
	// 		break;
	// 	}
	// }
	return 0;
}

/*****************************************************************************
 *                                block
 *****************************************************************************/
/**
 * <Ring 0> This routine is called after `p_flags' has been set (!= 0), it
 * calls `schedule()' to choose another proc as the `proc_ready'.
 *
 * @attention This routine does not change `p_flags'. Make sure the `p_flags'
 * of the proc to be blocked has been set properly.
 * 
 * @param p The proc to be blocked.
 *****************************************************************************/
PRIVATE void block(proc_task_struct_t* p)
{
	p->status = _PROC_SLEEP;
	assert(p->p_flags);
	proc_schedule();
	/* 自旋锁 */
	// while (p->status != _PROC_RUN);
}

/*****************************************************************************
 *                                unblock
 *****************************************************************************/
/**
 * <Ring 0> This is a dummy routine. It does nothing actually. When it is
 * called, the `p_flags' should have been cleared (== 0).
 * 
 * @param p The unblocked proc.
 *****************************************************************************/
PRIVATE void unblock(proc_task_struct_t* p)
{
	assert(p->p_flags == 0);
	p->status = _PROC_RUN;
}




/*****************************************************************************
 *                                msg_send
 *****************************************************************************/
/**
 * <Ring 0> Send a message to the dest proc. If dest is blocked waiting for
 * the message, copy the message to it and unblock dest. Otherwise the caller
 * will be blocked and appended to the dest's sending queue.
 * 
 * @param current  The caller, the sender.
 * @param dest     To whom the message is sent.
 * @param m        The message.
 * 
 * @return Zero if success.
 *****************************************************************************/
PRIVATE int msg_send(proc_task_struct_t * current, int dest, message_t * m)
{
	proc_task_struct_t * sender = current;
	proc_task_struct_t * p_dest = g_pcb_table + dest; /* proc dest */

	assert(proc2pid(sender) != dest);

	/* check for deadlock here */
	if (deadlock(proc2pid(sender), dest)) {
		panic(">>DEADLOCK<< %s->%s", sender->p_name, p_dest->p_name);
	}

	if ((p_dest->p_flags & RECEIVING) && /* dest is waiting for the msg */
	    (p_dest->p_recvfrom == proc2pid(sender) ||
	     p_dest->p_recvfrom == ANY)) {
		assert(p_dest->p_msg);
		assert(m);

		com_memncpy(va2la(dest, p_dest->p_msg),
			        va2la(proc2pid(sender), m),
			        sizeof(message_t));
		p_dest->p_msg = 0;
		p_dest->p_flags &= ~RECEIVING; /* dest has received the msg */
		p_dest->p_recvfrom = NO_TASK;
		unblock(p_dest);

		assert(p_dest->p_flags == 0);
		assert(p_dest->p_msg == 0);
		assert(p_dest->p_recvfrom == NO_TASK);
		assert(p_dest->p_sendto == NO_TASK);
		assert(sender->p_flags == 0);
		assert(sender->p_msg == 0);
		assert(sender->p_recvfrom == NO_TASK);
		assert(sender->p_sendto == NO_TASK);
	}
	else { /* dest is not waiting for the msg */
		sender->p_flags |= SENDING;
		assert(sender->p_flags == SENDING);
		sender->p_sendto = dest;
		sender->p_msg = m;

		/* append to the sending queue */
		proc_task_struct_t  * p;
		if (p_dest->q_sending) {
			p = p_dest->q_sending;
			while (p->next_sending)
				p = p->next_sending;
			p->next_sending = sender;
		}
		else {
			p_dest->q_sending = sender;
		}
		sender->next_sending = 0;

		block(sender);

		assert(sender->p_flags == SENDING);
		assert(sender->p_msg != 0);
		assert(sender->p_recvfrom == NO_TASK);
		assert(sender->p_sendto == dest);
	}

	return 0;
}


/*****************************************************************************
 *                                msg_receive
 *****************************************************************************/
/**
 * <Ring 0> Try to get a message from the src proc. If src is blocked sending
 * the message, copy the message from it and unblock src. Otherwise the caller
 * will be blocked.
 * 
 * @param current The caller, the proc who wanna receive.
 * @param src     From whom the message will be received.
 * @param m       The message ptr to accept the message.
 * 
 * @return  Zero if success.
 *****************************************************************************/
PRIVATE int msg_receive(proc_task_struct_t * current, int src, message_t * m)
{
	proc_task_struct_t * p_who_wanna_recv = current; /**
						  * This name is a little bit
						  * wierd, but it makes me
						  * think clearly, so I keep
						  * it.
						  */
	proc_task_struct_t * p_from = 0; /* from which the message will be fetched */
	proc_task_struct_t * prev = 0;
	int copyok = 0;

	assert(proc2pid(p_who_wanna_recv) != src);

	/* TODO:继续看吧？ */
	if ((p_who_wanna_recv->has_int_msg) &&
	    ((src == ANY) || (src == INTERRUPT))) {
		/* There is an interrupt needs p_who_wanna_recv's handling and
		 * p_who_wanna_recv is ready to handle it.
		 */

		message_t  msg;
		msg_reset(&msg);
		msg.source = INTERRUPT;
		msg.type = HARD_INT;
		assert(m);
		com_memncpy(va2la(proc2pid(p_who_wanna_recv), m), &msg,
			  sizeof(message_t ));

		p_who_wanna_recv->has_int_msg = 0;

		assert(p_who_wanna_recv->p_flags == 0);
		assert(p_who_wanna_recv->p_msg == 0);
		assert(p_who_wanna_recv->p_sendto == NO_TASK);
		assert(p_who_wanna_recv->has_int_msg == 0);

		return 0;
	}


	/* Arrives here if no interrupt for p_who_wanna_recv. */
	/* 确定p_from, 在队列中找到一个用于接受消息的进程指针 */
	if (src == ANY) {
		/* p_who_wanna_recv is ready to receive messages from
		 * ANY proc, we'll check the sending queue and pick the
		 * first proc in it.
		 */
		if (p_who_wanna_recv->q_sending) {
			p_from = p_who_wanna_recv->q_sending;
			copyok = 1;

			assert(p_who_wanna_recv->p_flags == 0);
			assert(p_who_wanna_recv->p_msg == 0);
			assert(p_who_wanna_recv->p_recvfrom == NO_TASK);
			assert(p_who_wanna_recv->p_sendto == NO_TASK);
			assert(p_who_wanna_recv->q_sending != 0);
			assert(p_from->p_flags == SENDING);
			assert(p_from->p_msg != 0);
			assert(p_from->p_recvfrom == NO_TASK);
			assert(p_from->p_sendto == proc2pid(p_who_wanna_recv));
		}
	}
	else {
		/* p_who_wanna_recv wants to receive a message from
		 * a certain proc: src.
		 */
		p_from = &g_pcb_table[src];

		if ((p_from->p_flags & SENDING) &&
		    (p_from->p_sendto == proc2pid(p_who_wanna_recv))) {
			/* Perfect, src is sending a message to
			 * p_who_wanna_recv.
			 */
			copyok = 1;

			proc_task_struct_t * p = p_who_wanna_recv->q_sending;
			assert(p); /* p_from must have been appended to the
				    * queue, so the queue must not be NULL
				    */
			/* 遍历队列，找到发送消息的源进程 */
			while (p) {
				assert(p_from->p_flags & SENDING);
				/* 如果p就是我要收的源进程，就不用找了 */
				if (proc2pid(p) == src) { /* if p is the one */
					p_from = p;
					break;
				}
				prev = p;
				p = p->next_sending;
			}

			assert(p_who_wanna_recv->p_flags == 0);
			assert(p_who_wanna_recv->p_msg == 0);
			assert(p_who_wanna_recv->p_recvfrom == NO_TASK);
			assert(p_who_wanna_recv->p_sendto == NO_TASK);
			assert(p_who_wanna_recv->q_sending != 0);
			assert(p_from->p_flags == SENDING);
			assert(p_from->p_msg != 0);
			assert(p_from->p_recvfrom == NO_TASK);
			assert(p_from->p_sendto == proc2pid(p_who_wanna_recv));
		}
	}

	/* 标注了copyok，意味着已经找到了p_from, 确定了源进程，并且源进程已经准备好了消息，可以将消息复制过来 */
	if (copyok) {
		/* It's determined from which proc the message will
		 * be copied. Note that this proc must have been
		 * waiting for this moment in the queue, so we should
		 * remove it from the queue.
		 */

		/* 维护待接收信息的进程队列 */
		/* 如果要接受的进程位于队列头，则由于队列头需要从队列中除去，需要做一些处理 */
		if (p_from == p_who_wanna_recv->q_sending) { /* the 1st one */
			assert(prev == 0);
			p_who_wanna_recv->q_sending = p_from->next_sending;
			p_from->next_sending = 0;
		}
		/* 如果不是队列头，则源进程的前一项和后一项接在一次 */
		else {
			assert(prev);
			prev->next_sending = p_from->next_sending;
			p_from->next_sending = 0;
		}

		assert(m);
		assert(p_from->p_msg);
		/* copy the message */
		com_memncpy(va2la(proc2pid(p_who_wanna_recv), m),
			  va2la(proc2pid(p_from), p_from->p_msg),
			  sizeof(message_t ));

		p_from->p_msg = 0;
		p_from->p_sendto = NO_TASK;
		p_from->p_flags &= ~SENDING;
		unblock(p_from);
	}
	/* 从已有的接收消息等待队列中，找不到自己想要的信息 */
	else {  /* nobody's sending any msg */
		/* Set p_flags so that p_who_wanna_recv will not
		 * be scheduled until it is unblocked.
		 */
		p_who_wanna_recv->p_flags |= RECEIVING;

		p_who_wanna_recv->p_msg = m;

		if (src == ANY)
			p_who_wanna_recv->p_recvfrom = ANY;
		else
			p_who_wanna_recv->p_recvfrom = proc2pid(p_from);

		/* block住，然后退出系统调用后时钟中断会使该进程被阻塞 */
		block(p_who_wanna_recv);

		assert(p_who_wanna_recv->p_flags == RECEIVING);
		assert(p_who_wanna_recv->p_msg != 0);
		assert(p_who_wanna_recv->p_recvfrom != NO_TASK);
		assert(p_who_wanna_recv->p_sendto == NO_TASK);
		assert(p_who_wanna_recv->has_int_msg == 0);
	}
	return 0;
}





/**
 * @brief 实现发送接受的内核代码
 * 
 * @param function 
 * @param src_dest 
 * @param m 
 * @param p 发送时，为发送源，接收时，为接受目的
 * @return PUBLIC sys_sendrec 
 */
PUBLIC int sys_sendrec(int function, int src_dest, message_t* m, proc_task_struct_t* p)
{
    int a = 0;
    // com_printk("in the sys_sendrec\n");
	// assert(k_reenter == 0);	/* make sure we are not in ring0 */
	// assert((src_dest >= 0 && src_dest < NR_TASKS + NR_PROCS) ||
	//        src_dest == ANY ||
	//        src_dest == INTERRUPT);

	int ret = 0;
    /* 因为调用者所在进程的地址空间和内核地址空间可能不一致，需要通过进程虚拟地址获得实际线性地址，这样子在内核就能够直接对用户地址空间内的信息直接操作。 */
	int caller = proc2pid(p);
	message_t * mla = (message_t *)va2la(caller, m);
	mla->source = caller;

	// assert(mla->source != src_dest);

	/**
	 * Actually we have the third message type: BOTH. However, it is not
	 * allowed to be passed to the kernel directly. Kernel doesn't know
	 * it at all. It is transformed into a SEND followed by a RECEIVE
	 * by `send_recv()'.
	 */
	if (function == SEND) {
		ret = msg_send(p, src_dest, m);
		if (ret != 0)
			return ret;
	}
	else if (function == RECEIVE) {
		ret = msg_receive(p, src_dest, m);
		if (ret != 0)
			return ret;
	}
	else {
		panic("{sys_sendrec} invalid function: "
		      "%d (SEND:%d, RECEIVE:%d).", function, SEND, RECEIVE);
	}

	return 0;
}