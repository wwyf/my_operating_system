#include <process.h>
#include <const.h>


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

/**
 * @brief  初始化一个内核进程，初始化其eip，cs，其他使用内核常量填充。这样就能够跳转过去运行。
 * 
 * @param n 该进程在进程控制块中的序号
 * @param name 进程名字
 * @param pid 如名
 * @param function 该进程的入口函数（即入口地址）
 */
void init_a_process(uint32_t n, char * name, uint32_t pid, void * function){
    PCB_table[n].regs.eax = 0;
    PCB_table[n].regs.ebx = 0;
    PCB_table[n].regs.ecx = 0;
    PCB_table[n].regs.edx = 0;
    PCB_table[n].regs.edi = 0;
    PCB_table[n].regs.esi = 0;
    PCB_table[n].regs.ebp = 0;

    PCB_table[n].regs.xfs = __KERNEL_FS;
    PCB_table[n].regs.xds = __KERNEL_DS;
    PCB_table[n].regs.xes = __KERNEL_ES;

    PCB_table[n].regs.xcs = __KERNEL_CS;
    PCB_table[n].regs.xss = __KERNEL_SS;
    PCB_table[n].regs.esp = 0;// TODO:
    PCB_table[n].regs.eip = (uint32_t)function;
    PCB_table[n].regs.eflags = 0; // TODO:

    PCB_table[n].pid = pid;

    // PCB_table[n].p_name = "test";


}