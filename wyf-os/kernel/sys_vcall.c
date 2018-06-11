/* 因为send和recv才是真正的系统调用，这里的都是在这真正系统调用的基础上实现的与linux相似的系统的调用，应该说是virtual sys call */
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
