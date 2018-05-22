#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <type.h>
#include <const.h>

/* 用来保存CPU的上下文信息 */
typedef struct pt_regs{
    uint32_t ebx;
    uint32_t ecx; 
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t eax;
    uint16_t xds;
    uint16_t xes;
    uint16_t xfs;
    // 中断号
    uint32_t orig_eax;
    // 返回地址信息
    uint32_t eip;
    uint16_t xcs;
    uint32_t eflags;
    uint32_t esp;
    uint16_t xss;
}proc_regs_t __attribute__((gcc_struct, packed));


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

