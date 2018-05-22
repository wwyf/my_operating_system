#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <type.h>

#define MAX_PROCESS_NUM 4

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
}PT_REGS;


// typedef struct s_stackframe{
//     uint32_t ebx;
//     uint32_t ecx;
//     uint32_t edx;
//     uint32_t esi;
//     uint32_t edi;
//     uint32_t ebp;
//     uint32_t eax;
//     uint16_t xds;
//     uint16_t xes;
//     uint16_t xfs;
//     // 中断号
//     uint32_t orig_eax;
//     // 返回地址信息
//     uint32_t eip;
//     uint16_t xcs;
//     uint32_t eflags;
//     uint32_t esp;
//     uint16_t xss;
// }STACK_FRAME;

typedef struct s_proc{
    PT_REGS regs;
    uint32_t pid;
    char p_name[16];
    void * kernel_stack;
}PROCESS;

PROCESS PCB_table[MAX_PROCESS_NUM];
PROCESS * current;
PT_REGS * current_process_kernel_stack;

void process_init();

void update_current_process_context(PT_REGS * regs);


#endif

