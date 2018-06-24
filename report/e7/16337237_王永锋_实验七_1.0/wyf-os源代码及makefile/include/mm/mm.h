#ifndef _MM_H_

#include <type.h>
#include <const.h>

/* 内存管理进程 */
PUBLIC void task_mm();

PUBLIC int do_fork();

PUBLIC void do_exit(int status);

PUBLIC void do_wait();

PUBLIC int mm_alloc_mem_default(int pid);

extern message_t mm_msg;

#endif // !_MM_H_