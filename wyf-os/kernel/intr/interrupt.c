#include <intr/interrupt.h>
#include <global.h>
#include <protect/protect.h>
#include <proc/process.h>
#include <common/debug.h>
#include <common/debug.h>
#include <common/stdlib.h>



void _interrupt_handler(proc_regs_t * regs){
    // 将上下文保存到当前进程控制块中。
    update_current_process_context(regs);

    int v = regs->orig_eax;
    com_print("in the %d interrupt!!!", v);
    
}

void interrupt_init(){
    for (int i = 0; i < 256; i++){
        // com_print("%d  ", interrupt_table[i]);
        // com_printk("%x  ", ((uint32_t)&interrupt_table + 8 * i));
        set_intr_gate(i, (void *)((uint32_t)&interrupt_table + 8 * i));
    }
}