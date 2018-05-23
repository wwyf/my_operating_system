#include <global.h>
#include <protect/protect.h>
#include <proc/process.h>
#include <proc/schedule.h>
#include <common/debug.h>
#include <common/debug.h>
#include <common/stdlib.h>

#include <intr/interrupt.h>

/**
 * @brief 将获得的上下文信息（寄存器）保存到当前进程中
 * 
 * @param regs 一个proc_regs_t类型的结构体指针
 */
void _update_current_process_context(proc_regs_t * regs);


void interrupt_init(){
    for (int i = 0; i < 256; i++){
        set_intr_gate(i, (void *)((uint32_t)&interrupt_table + 8 * i));
    }
}

/**
 * @brief 可能是个hack？传参方式为在汇编中push
 * 
 * @param regs 当前进程上下文
 */
void _interrupt_handler(proc_regs_t * regs){
    // 将上下文保存到当前进程控制块中。
    _update_current_process_context(regs);
    uint32_t v = regs->orig_eax;
    
    switch (v){
        case 0x66:{
            proc_schedule();
            break;
        }
        default:{
            com_printk("in the %d interrupt!", v);
        }
    }
    
}


/**
 * @brief 将当前进程的上下文更新进当前进程的进程控制块中
 * 
 * @param regs 指针，指向当前进程内核堆栈中的上下文首地址
 */
void _update_current_process_context(proc_regs_t * regs){
    g_cur_proc_context_stack = regs;
    com_memcpy((char *)regs, (char *)&g_cur_proc->regs, sizeof(proc_regs_t));
    g_cur_proc->kernel_stack = regs;
}
