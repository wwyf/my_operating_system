#include <interrupt.h>
#include <head.h>
#include <protect.h>
#include <debug.h>
#include <kernel.h>
#include <process.h>
#include <stdlib.h>



void interrupt_handler(PT_REGS * regs){
    // 将上下文保存到进程控制块中。
    update_current_process_context(regs);

    int v = regs->orig_eax;
    print("in the %d interrupt!!!", v);
    


}

void interrupt_init(){
    for (int i = 0; i < 256; i++){
        // print("%d  ", interrupt_table[i]);
        // printk("%x  ", ((uint32_t)&interrupt_table + 8 * i));
        set_intr_gate(i, (void *)((uint32_t)&interrupt_table + 8 * i));
    }
}
