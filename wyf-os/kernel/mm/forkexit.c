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
	proc->status = _PROC_EMPTY;
}


/*****************************************************************************
 *                                do_fork
 *****************************************************************************/
/**
 * Perform the fork() syscall.
 * 
 * @return  Zero if success
 *****************************************************************************/
PUBLIC int do_fork()
{
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
    com_printk("pid(%d) : do fork! fork to pid(%d)\n", father_pid, child_pid);

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
}

/*****************************************************************************
 *                                do_exit
 *****************************************************************************/
PUBLIC void do_exit(int status)
{
	int i;
	int pid = mm_msg.source; /* PID of caller */
	int parent_pid = g_pcb_table[pid].p_parent;
	com_printk("pid(%d) : do exit! My father is pid(%d)\n", pid, parent_pid);
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
			com_printk("pid(%d) : do wait! I have children pid(%d) status(%d)\n", pid, p_proc->pid, p_proc->p_flags);
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
		com_printk("pid(%d) : do wait! I don't have children\n", pid);
		message_t msg;
		msg.type = SYSCALL_RET;
		msg.PID = NO_TASK;
		msg_send_recv(SEND, pid, &msg);
	}
}
