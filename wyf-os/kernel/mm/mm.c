#include <mm/mm.h>
#include <message.h>
PRIVATE void init_mm();
PUBLIC int mm_alloc_mem_default(int pid);

message_t mm_msg;

PUBLIC void task_mm()
{
	init_mm();
	
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



/*****************************************************************************
 *                                alloc_mem
 *****************************************************************************/
/**
 * Allocate a memory block for a proc.
 * 
 * @param pid  Which proc the memory is for.
 * @param memsize  How many bytes is needed.
 * 
 * @return  The base of the memory just allocated.
 *****************************************************************************/
PUBLIC int mm_alloc_mem_default(int pid)
{
	/* 从0x40000开始分配， 给每个进程分配固定大小0x4000字节*/
	int base = 0x20000 + pid * 0x5000;
	return base;
}
