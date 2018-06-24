/*************************************************************************//**
 *****************************************************************************
 * @file   forkexit.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Tue May  6 00:37:15 2008
 *****************************************************************************
 *****************************************************************************/
#include <mm/mm.h>
#include <common/common.h>
#include <proc/process.h>
#include <global.h>
#include <proc/process.h>
#include <message.h>
#include <global.h>

PRIVATE void cleanup(proc_task_struct_t * proc);



/*****************************************************************************
 *                                cleanup
 *
 * 
 */

PRIVATE void cleanup(proc_task_struct_t * proc)
{
	message_t msg2parent;
	msg2parent.type = SYSCALL_RET;
	msg2parent.PID = proc->pid;
	msg2parent.STATUS = proc->exit_status;
	msg_send_recv(SEND, proc->p_parent, &msg2parent);

	proc->p_flags = FREE_SLOT;
}


/*****************************************************************************
 *                                do_fork
 *****************************************************************************/
/**
 * Perform the fork() syscall.
 * 
 * @return  Zero if success, otherwise -1.
 *****************************************************************************/
PUBLIC int do_fork()
{
    com_printk("do fork!");
	/* find a free slot in proc_table */
	proc_task_struct_t * cur_proc_table = g_pcb_table;
    int cur_empty_pcb_pid = 0;
    for (int i = 5; i < _PROC_NUM; i++){
        if (cur_proc_table[i].status == _PROC_EMPTY){
            cur_empty_pcb_pid = i;
            break;
        }
    }
	int father_pid = mm_msg.source;
	proc_task_struct_t * father_proc = &g_pcb_table[father_pid];
    int child_pid = cur_empty_pcb_pid;
    proc_task_struct_t * child_proc = &g_pcb_table[child_pid];

    /* 得到了 cur_empty_pcb_pid， 等下就把进程控制块复制一遍，放到这个pcb上 */
    com_memncpy(child_proc, father_proc, sizeof(proc_task_struct_t));

    /* 分配栈空间，并将父进程的栈复制到子进程的栈中 */
    uint32_t new_stack = mm_alloc_mem_default(cur_empty_pcb_pid);
    // 栈复制 源地址
    uint32_t src = (uint32_t)father_proc->kernel_stack;
    // 栈偏移量
    uint32_t offset = father_proc->stack_base + sizeof(proc_regs_t) - src;
    // 栈复制 目的地址
    uint32_t dest = new_stack + sizeof(proc_regs_t) - offset;
    com_memncpy((void *)dest, (void *)src, offset);

    /* 修改子进程控制块，子进程使用新栈 */
    child_proc->kernel_stack = (void *)dest;
	child_proc->pid = child_pid;
	child_proc->p_parent = father_pid;

	/* child PID will be returned to the parent proc */
	mm_msg.PID = child_pid;

	/* birth of the child */
	message_t m;
	m.type = SYSCALL_RET;
	m.RETVAL = 0;
	m.PID = 0;
	msg_send_recv(SEND, child_pid, &m);

    return 0;
	// int i;
	// for (i = 0; i < NR_TASKS + NR_PROCS; i++,p++)
	// 	if (p->p_flags == FREE_SLOT)
	// 		break;

	// int child_pid = i;
	// assert(p == &proc_table[child_pid]);
	// assert(child_pid >= NR_TASKS + NR_NATIVE_PROCS);
	// if (i == NR_TASKS + NR_PROCS) /* no free slot */
	// 	return -1;
	// assert(i < NR_TASKS + NR_PROCS);

	// /* duplicate the process table */
	// int pid = mm_msg.source;
	// u16 child_ldt_sel = p->ldt_sel;
	// *p = proc_table[pid];
	// p->ldt_sel = child_ldt_sel;
	// p->p_parent = pid;
	// sprintf(p->name, "%s_%d", proc_table[pid].name, child_pid);

	// /* duplicate the process: T, D & S */
	// struct descriptor * ppd;

	// /* Text segment */
	// ppd = &proc_table[pid].ldts[INDEX_LDT_C];
	// /* base of T-seg, in bytes */
	// int caller_T_base  = reassembly(ppd->base_high, 24,
	// 				ppd->base_mid,  16,
	// 				ppd->base_low);
	// /* limit of T-seg, in 1 or 4096 bytes,
	//    depending on the G bit of descriptor */
	// int caller_T_limit = reassembly(0, 0,
	// 				(ppd->limit_high_attr2 & 0xF), 16,
	// 				ppd->limit_low);
	// /* size of T-seg, in bytes */
	// int caller_T_size  = ((caller_T_limit + 1) *
	// 		      ((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?
	// 		       4096 : 1));

	// /* Data & Stack segments */
	// ppd = &proc_table[pid].ldts[INDEX_LDT_RW];
	// /* base of D&S-seg, in bytes */
	// int caller_D_S_base  = reassembly(ppd->base_high, 24,
	// 				  ppd->base_mid,  16,
	// 				  ppd->base_low);
	// /* limit of D&S-seg, in 1 or 4096 bytes,
	//    depending on the G bit of descriptor */
	// int caller_D_S_limit = reassembly((ppd->limit_high_attr2 & 0xF), 16,
	// 				  0, 0,
	// 				  ppd->limit_low);
	// /* size of D&S-seg, in bytes */
	// int caller_D_S_size  = ((caller_T_limit + 1) *
	// 			((ppd->limit_high_attr2 & (DA_LIMIT_4K >> 8)) ?
	// 			 4096 : 1));

	// /* we don't separate T, D & S segments, so we have: */
	// assert((caller_T_base  == caller_D_S_base ) &&
	//        (caller_T_limit == caller_D_S_limit) &&
	//        (caller_T_size  == caller_D_S_size ));

	// /* base of child proc, T, D & S segments share the same space,
	//    so we allocate memory just once */
	// int child_base = alloc_mem(child_pid, caller_T_size);
	// /* int child_limit = caller_T_limit; */
	// printl("{MM} 0x%x <- 0x%x (0x%x bytes)\n",
	//        child_base, caller_T_base, caller_T_size);
	// /* child is a copy of the parent */
	// phys_copy((void*)child_base, (void*)caller_T_base, caller_T_size);

	// /* child's LDT */
	// init_desc(&p->ldts[INDEX_LDT_C],
	// 	  child_base,
	// 	  (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT,
	// 	  DA_LIMIT_4K | DA_32 | DA_C | PRIVILEGE_USER << 5);
	// init_desc(&p->ldts[INDEX_LDT_RW],
	// 	  child_base,
	// 	  (PROC_IMAGE_SIZE_DEFAULT - 1) >> LIMIT_4K_SHIFT,
	// 	  DA_LIMIT_4K | DA_32 | DA_DRW | PRIVILEGE_USER << 5);

	// /* tell FS, see fs_fork() */
	// MESSAGE msg2fs;
	// msg2fs.type = FORK;
	// msg2fs.PID = child_pid;
	// msg_send_recv(BOTH, TASK_FS, &msg2fs);

	// /* child PID will be returned to the parent proc */
	// mm_msg.PID = child_pid;

	// /* birth of the child */
	// MESSAGE m;
	// m.type = SYSCALL_RET;
	// m.RETVAL = 0;
	// m.PID = 0;
	// msg_send_recv(SEND, child_pid, &m);

	// return 0;
}

/*****************************************************************************
 *                                do_exit
 *****************************************************************************/
/**
 * Perform the exit() syscall.
 *
 * If proc A calls exit(), then MM will do the following in this routine:
 *     <1> inform FS so that the fd-related things will be cleaned up
 *     <2> tell TASK_SYS (TODO)
 *     <3> free A's memory
 *     <4> set A.exit_status, which is for the parent
 *     <5> depends on parent's status. if parent (say P) is:
 *           (1) WAITING
 *                 - clean P's WAITING bit, and
 *                 - send P a message to unblock it
 *                   {P's wait() call is done}
 *                 - release A's proc_table[] slot
 *                   {A's exit() call is done}
 *           (2) not WAITING
 *                 - set A's HANGING bit
 *                   {things will be done at do_wait()::comment::<1>}
 *     <6> iterate proc_table[], if proc B is found as A's child, then:
 *           (1) make INIT the new parent of B, and
 *           (2) if INIT is WAITING and B is HANGING, then:
 *                 - clean INIT's WAITING bit, and
 *                 - send INIT a message to unblock it
 *                   {INIT's wait() call is done}
 *                 - release B's proc_table[] slot
 *                   {B's exit() call is done}
 *               else
 *                 if INIT is WAITING but B is not HANGING, then
 *                     - B will call exit() and things will be done at
 *                       do_exit()::comment::<5>::(1)
 *                 if B is HANGING but INIT is not WAITING, then
 *                     - INIT will call wait() and things will be doen at
 *                       do_wait()::comment::<1>
 *
 * TERMs:
 *     - HANGING: everything except the proc_table entry has been cleaned up.
 *     - WAITING: a proc has at least one child, and it is waiting for the
 *                child(ren) to exit()
 *     - zombie: say P has a child A, A will become a zombie if
 *         - A exit(), and
 *         - P does not wait(), neither does it exit(). that is to say, P just
 *           keeps running without terminating itself or its child
 *         - @see Linux manpage for more details:
 *             - man 2 exit
 *             - man 3 exit
 *             - man 2 wait
 * 
 * @param status  Exiting status for parent.
 * 
 *****************************************************************************/
PUBLIC void do_exit(int status)
{
	com_printk("do exit!");
	int i;
	int pid = mm_msg.source; /* PID of caller */
	int parent_pid = g_pcb_table[pid].p_parent;
	proc_task_struct_t * p = &g_pcb_table[pid];

	/* 应该有一个清楚内存的函数 */

	p->exit_status = status;

	if (g_pcb_table[parent_pid].p_flags & WAITING) { /* parent is waiting */
		g_pcb_table[parent_pid].p_flags &= ~WAITING;
		cleanup(&g_pcb_table[pid]);
	}
	else { /* parent is not waiting */
		g_pcb_table[pid].p_flags |= HANGING;
	}

	/* if the proc has any child, make INIT the new parent */
	for (i = 0; i < _PROC_NUM; i++) {
		if (g_pcb_table[i].p_parent == pid) { /* is a child */
			g_pcb_table[i].p_parent = TASK_INIT;
			if ((g_pcb_table[TASK_INIT].p_flags & WAITING) &&
			    (g_pcb_table[i].p_flags & HANGING)) {
				g_pcb_table[TASK_INIT].p_flags &= ~WAITING;
				cleanup(&g_pcb_table[i]);
			}
		}
	}
}



/*****************************************************************************
 *                                do_wait
 *****************************************************************************/
PUBLIC void do_wait()
{
	com_printk("do wait!");
	int pid = mm_msg.source;

	int i;
	int children = 0;
	proc_task_struct_t * p_proc = g_pcb_table;
	/* 
	遍历进程控制块列表，寻找该进程的所有子进程
	如果找到了子进程，并且该子进程还处于handing状态，就清除该进程
	*/
	for (i = 0; i < _PROC_NUM; i++,p_proc++) {
		if (p_proc->p_parent == pid) {
			children++;
			if (p_proc->p_flags & HANGING) {
				cleanup(p_proc);
				return;
			}
		}
	}

	if (children) {
		/* has children, but no child is HANGING */
		g_pcb_table[pid].p_flags |= WAITING;
	}
	else {
		/* no child at all */
		message_t msg;
		msg.type = SYSCALL_RET;
		msg.PID = NO_TASK;
		msg_send_recv(SEND, pid, &msg);
	}
}
