#include <interrupt.h>
#include <head.h>
#include <protect.h>
#include <debug.h>
#include <kernel.h>
#include <process.h>
#include <stdlib.h>



void interrupt_handler(PT_REGS * regs){
    int v = regs->orig_eax;
    print("in the %d interrupt!!!", v);
    print("%x ", regs->ebp);

    // 将上下文保存到进程控制块中。
    update_current_process_context(regs);

    print("test current!!  %d", current->regs.orig_eax);
    print("test current-stack!! %d", current_process_kernel_stack->eip);
    // while (1){}
    // TODO:将这个结构体的内容复制到进程控制块中。
    // TODO:然后做其他的事情
    // do something


    //TODO:获得新进程的内核栈指针
}

void interrupt_init(){
    for (int i = 0; i < 256; i++){
        // print("%d  ", interrupt_table[i]);
        // printk("%x  ", ((uint32_t)&interrupt_table + 8 * i));
        set_intr_gate(i, (void *)((uint32_t)&interrupt_table + 8 * i));
    }
}