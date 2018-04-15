#ifndef _PROC_H_
#define _PROC_H_

typedef struct proc_register {
    u32 gs;
    u32 fs;
    u32 ds;
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 kernel_esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;
    u32 retaddr;
    u32 eip;
    u32 cs;
    u32 eflags;
    u32 esp;
    u32 ss;
}procRegister;


typedef struct controlProcessBlock{
    proc_register regs;
    u32 pid;
    char p_name[16];
}PCB_t



#endif