#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <type.h>
#include <const.h>

/* 用来保存CPU的上下文信息 */
typedef struct pt_regs{
    long ebx;
    long ecx; 
    long edx;
    long esi;
    long edi;
    long ebp;
    long eax;
    int xds;
    int xes;
    int xfs;
    // 中断号
    long orig_eax;
    // 返回地址信息
    long eip;
    int xcs;
    long eflags;
    long esp;
    int xss;
}proc_regs_t;


/* 这就是PCB啦，进程控制块 */
typedef struct s_proc{
    proc_regs_t regs;
    uint32_t pid;
    char p_name[16];
    void * kernel_stack;
}proc_task_struct_t;


proc_task_struct_t g_pcb_table[MAX_PROCESS_NUM]; /// 进程控制块表。
proc_task_struct_t * g_cur_proc; /// 当前进程。
proc_regs_t * g_cur_proc_context_stack; /// 当前进程恢复上下文所用内核栈。

void process_init();

#endif

