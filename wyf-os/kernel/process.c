#include <process.h>


/**
 * @brief 初始化第一个进程
 * 
 */
void process_init(){
    current = &PCB_table[0];
    current_process_kernel_stack = &current->regs;
}



void update_current_process_context(PT_REGS * regs){
    current_process_kernel_stack = regs;
    memcpyk((char *)regs, (char *)&current->regs, sizeof(PT_REGS));
    current->kernel_stack = regs;
}