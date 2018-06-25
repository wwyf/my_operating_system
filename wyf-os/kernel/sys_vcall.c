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
message_t fork_msg;
message_t fork_msg2;
PUBLIC int user_fork()
{
	
	if (user_get_pid() <= 6){
		fork_msg.type = FORK;
		msg_send_recv(BOTH, TASK_MM, &fork_msg);
		// assert(fork_msg.type == SYSCALL_RET);
		// assert(fork_msg.RETVAL == 0);
		return fork_msg.PID;
	}
	else {
		fork_msg2.type = FORK;
		msg_send_recv(BOTH, TASK_MM, &fork_msg2);
		// assert(fork_msg.type == SYSCALL_RET);
		// assert(fork_msg.RETVAL == 0);
		return fork_msg2.PID;

	}

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


/**
 * @brief 申请信号量
 * 
 * 从信号量数组中寻找到可用信号量，并设置信号量初值，返回信号量ID。
 * 
 * @param sem_value 信号量初值
 * @return int 申请到的信号量的id
 */
PUBLIC int user_get_sem(int sem_value){
	message_t msg;
	msg.type = SEMAPHORE;
	msg.FLAGS = SEM_GETSEM;
	msg.VALUE = sem_value;

	msg_send_recv(BOTH, TASK_SYS, &msg);

	/* 返回 */
	return msg.RETVAL;
}

/**
 * @brief 释放信号量
 * 
 * 将信号量的使用标志设置为未使用。
 * 
 * @param sem_id 要释放的信号量的id
 * @return 
 */
PUBLIC int user_free_sem(int sem_id){
	message_t msg;
	msg.type = SEMAPHORE;
	msg.FLAGS = SEM_FREESEM;
	msg.VALUE = sem_id;

	msg_send_recv(BOTH, TASK_SYS, &msg);

	/* 返回 */
	return msg.RETVAL;
}

/**
 * @brief 信号量的P操作
 * 
 * 将信号量的值减去1
 * 如果信号量的值小于0，就阻塞当前进程。
 * 
 * @param sem_id 
 * @return 
 */
PUBLIC int user_sem_p(int sem_id){
	message_t msg;
	msg.type = SEMAPHORE;
	msg.FLAGS = SEM_P;
	msg.VALUE = sem_id;

	msg_send_recv(BOTH, TASK_SYS, &msg);

	/* 返回 */
	return msg.RETVAL;
}

/**
 * @brief 信号量的V操作
 * 
 * 将信号量的值+1
 * 如果信号量的值<=0，就唤醒一个新进程
 * 
 * @param sem_id 
 * @return PUBLIC user_sem_p 
 */
PUBLIC int user_sem_v(int sem_id){
	message_t msg;
	msg.type = SEMAPHORE;
	msg.FLAGS = SEM_V;
	msg.VALUE = sem_id;

	msg_send_recv(BOTH, TASK_SYS, &msg);

	/* 返回 */
	return msg.RETVAL;
}