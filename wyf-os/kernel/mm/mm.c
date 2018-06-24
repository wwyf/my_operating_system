#include <mm/mm.h>
#include <message.h>
PRIVATE void init_mm();

PUBLIC void task_mm()
{
	init_mm();
	message_t mm_msg;
	while (1) {
		msg_send_recv(RECEIVE, ANY, &mm_msg);
		int src = mm_msg.source;
		int reply = 1;

		int msgtype = mm_msg.type;

		switch (msgtype) {
		case FORK:
			mm_msg.RETVAL = do_fork();
			break;
		case EXIT:
			do_exit(mm_msg.STATUS);
			reply = 0;
			break;
		/* case EXEC: */
		/* 	mm_msg.RETVAL = do_exec(); */
		/* 	break; */
		case WAIT:
			do_wait();
			reply = 0;
			break;
		default:
			// dump_msg("MM::unknown msg", &mm_msg);
			assert(0);
			break;
		}

		if (reply) {
			mm_msg.type = SYSCALL_RET;
			msg_send_recv(SEND, src, &mm_msg);
		}
	}
}


/*****************************************************************************
 *                                init_mm
 *****************************************************************************/
/**
 * Do some initialization work.
 * 
 *****************************************************************************/
PRIVATE void init_mm()
{
	/* maybe do something */
	// struct boot_params bp;
	// get_boot_params(&bp);

	// memory_size = bp.mem_size;

	// /* print memory size */
	// printl("{MM} memsize:%dMB\n", memory_size / (1024 * 1024));
}

