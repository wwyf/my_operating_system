#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define	EXTERN
#endif

#include "../include/type.h"
#define MAX_SYSTEM_CALL 256
#define MAX_PROCESS_NUM 256

// 系统调用表
EXTERN u16 system_call[MAX_SYSTEM_CALL];

// 进程控制块表
EXTERN PCB_t process_table[MAX_SYSTEM_CALL];
// 维护当前进程对应的进程控制块的指针
EXTERN u16 cur_process_index;
EXTERN u16* cur_process;

// 当前运行进程号（对应进程控制块表）
EXTERN u16 current_process;