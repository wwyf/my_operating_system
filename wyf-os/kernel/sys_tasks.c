#include <common/common.h>
#include <sys_tasks.h>
#include <global.h>
#include <proc/process.h>
#include <sys_call.h>
#include <message.h>
#include <mm/mm.h>
#include <semaphore.h>

PRIVATE void task_sys_call();
PRIVATE void task_init_init();

/**
 * @brief 初始化系统的各项任务
 * 
 * 初始化1号进程为 任务：处理各项系统调用的进程
 */
PUBLIC void task_init(){
    proc_init_a_task(TASK_INIT, "init", TASK_INIT, (void*)task_init_init, 2);
    proc_init_a_task(TASK_SYS, "sys_call", TASK_SYS, (void*)task_sys_call, 2);
    proc_init_a_task(TASK_MM, "mm", TASK_MM, (void*)task_mm, 2);
}



/**
 * @brief 初始化init进程
 * 
 * @return PRIVATE init_init 
 */
PRIVATE void task_init_init()
{
	com_printk("Init() is running ...\n");
	// int pid = fork();
	// if (pid != 0) { /* parent process */
	// 	com_printk("parent is running, child pid:%d\n", pid);
	// 	// spin("parent");
	// }
	// else {	/* child process */
	// 	com_printk("child is running, pid:%d\n", getpid());
	// 	// spin("child");
	// }
	while (1){}
}



/**
 * @brief 系统任务
 * 
 * 该任务默认pid为1，并且作为常数不变。
 * 
 * @return PRIVATE task_sys_call 
 */
PRIVATE void task_sys_call(){
	message_t msg;
	while (1) {
		msg_send_recv(RECEIVE, ANY, &msg);
		int src = msg.source;

		switch (msg.type) {
		case GET_TICKS:
			msg.RETVAL = g_ticks;
			msg_send_recv(SEND, src, &msg);
			break;
		case GET_PID:
			msg.RETVAL = g_pcb_table[msg.source].pid;
			msg_send_recv(SEND, src, &msg);
			break;
		case SEMAPHORE:
			msg.RETVAL = sem_solve(msg.source, msg.FLAGS, msg.VALUE);
			msg_send_recv(SEND, src, &msg);
			break;
		default:{}
			panic("unknown msg type");
			break;
		}
	}
}

