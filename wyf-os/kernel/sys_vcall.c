/* 
因为send和recv才是真正的系统调用，这里的都是在这真正系统调用的基础上实现的与linux相似的系统的调用，应该说是virtual sys call 
*/
#include <sys_vcall.h>



PUBLIC int user_get_ticks(){
	message_t msg;
	msg_reset(&msg);
	msg.type = GET_TICKS;
    /* NOTICE: TASK_SYS 指的是ssy_task的进程号 */
	msg_send_recv(BOTH, TASK_SYS, &msg);
	// msg.RETVAL = g_ticks;
	return msg.RETVAL;
}

PUBLIC int user_get_pid(){
	message_t msg;
	msg_reset(&msg);
	msg.type = GET_PID;
    /* NOTICE: TASK_SYS 指的是ssy_task的进程号 */
	msg_send_recv(BOTH, TASK_SYS, &msg);
	// msg.RETVAL = g_ticks;
	return msg.RETVAL;

}

/*****************************************************************************
 *                                fork
 *****************************************************************************/
/**
 * Create a child process, which is actually a copy of the caller.
 * 
 * @return   On success, the PID of the child process is returned in the
 *         parent's thread of execution, and a 0 is returned in the child's
 *         thread of execution.
 *           On failure, a -1 will be returned in the parent's context, no
 *         child process will be created.
 *****************************************************************************/
PUBLIC int user_fork()
{
	message_t msg;
	msg.type = FORK;

	msg_send_recv(BOTH, TASK_MM, &msg);
	// assert(msg.type == SYSCALL_RET);
	// assert(msg.RETVAL == 0);

	return msg.PID;
}

PUBLIC int user_wait(int * status)
{
	message_t msg;
	msg.type   = WAIT;

	msg_send_recv(BOTH, TASK_MM, &msg);

	*status = msg.STATUS;

	return (msg.PID == NO_TASK ? -1 : msg.PID);
}

PUBLIC void user_exit(int status)
{
	message_t msg;
	msg.type	= EXIT;
	msg.STATUS	= status;

	msg_send_recv(BOTH, TASK_MM, &msg);
	// assert(msg.type == SYSCALL_RET);
}

