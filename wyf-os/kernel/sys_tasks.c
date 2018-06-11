#include <common/common.h>
#include <sys_tasks.h>
#include <global.h>
#include <proc/process.h>
#include <sys_call.h>
#include <message.h>

PRIVATE void task_sys_call();

/**
 * @brief 初始化系统的各项任务
 * 
 * 初始化1号进程为 任务：处理各项系统调用的进程
 */
PUBLIC void task_init(){
    proc_init_a_task(TASK_SYS, "sys_call", TASK_SYS, (void*)task_sys_call, (proc_regs_t*)0x20000, 2);
}

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
		default:{}
			panic("unknown msg type");
			break;
		}
	}
}
