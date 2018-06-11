#include <type.h>
#include <const.h>
#include <common/common.h>
#include <message.h>
/**
 * @brief 清空消息，使用0填充消息
 * 
 * @param p 
 * @return PUBLIC reset_msg 
 */
PUBLIC void msg_reset(message_t* p)
{
	com_memset(p, 0, sizeof(message_t));
}



/*****************************************************************************
 *                                send_recv
 *****************************************************************************/
/**
 * <Ring 1~3> IPC syscall.
 *
 * It is an encapsulation of `sendrec',
 * invoking `sendrec' directly should be avoided
 *
 * @param function  SEND, RECEIVE or BOTH
 * @param src_dest  The caller's proc_nr
 * @param msg       Pointer to the MESSAGE struct
 * 
 * @return always 0.
 *****************************************************************************/
PUBLIC int msg_send_recv(int function, int src_dest, message_t* msg)
{
	int ret = 0;

	if (function == RECEIVE)
		com_memset(msg, 0, sizeof(message_t));

	switch (function) {
	case BOTH:
		ret = sendrec(SEND, src_dest, msg);
		if (ret == 0)
			ret = sendrec(RECEIVE, src_dest, msg);
		break;
	case SEND:
	case RECEIVE:
		ret = sendrec(function, src_dest, msg);
		break;
	default:
		assert((function == BOTH) ||
		       (function == SEND) || (function == RECEIVE));
		break;
	}

	return ret;
}
