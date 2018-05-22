#ifndef _PTRACE_H_
#define _PTRACE_H_


struct pt_regs{
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
};

#endif
