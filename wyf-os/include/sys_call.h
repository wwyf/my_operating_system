#ifndef _SYS_CALL_H_
#define _SYS_CALL_H_

#include <type.h>
#include <const.h>
#include <proc/process.h>

PUBLIC void sys_call(proc_regs_t * regs);

PUBLIC int sys_sendrec(int function, int src_dest, message_t* m, proc_task_struct_t* p);

#endif // !_SYS_CALL_H_
