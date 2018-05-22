#include <common/common.h>
#include <proc/process.h>
#include <const.h>

void _init_a_process(uint32_t n, char * name, uint32_t pid, void * function);


/**
 * @brief 初始化第一个进程
 * 
 */
void process_init(){
    g_cur_proc = &g_pcb_table[0];
    g_cur_proc_context_stack = &g_cur_proc->regs;
}



/**
 * @brief  初始化一个内核进程，初始化其eip，cs，其他使用内核常量填充。这样就能够跳转过去运行。
 * 
 * @param n 该进程在进程控制块中的序号
 * @param name 进程名字
 * @param pid 如名
 * @param function 该进程的入口函数（即入口地址）
 */
void _init_a_process(uint32_t n, char * name, uint32_t pid, void * function){
    g_pcb_table[n].regs.eax = 0;
    g_pcb_table[n].regs.ebx = 0;
    g_pcb_table[n].regs.ecx = 0;
    g_pcb_table[n].regs.edx = 0;
    g_pcb_table[n].regs.edi = 0;
    g_pcb_table[n].regs.esi = 0;
    g_pcb_table[n].regs.ebp = 0;

    g_pcb_table[n].regs.xfs = __KERNEL_FS;
    g_pcb_table[n].regs.xds = __KERNEL_DS;
    g_pcb_table[n].regs.xes = __KERNEL_ES;

    g_pcb_table[n].regs.xcs = __KERNEL_CS;
    g_pcb_table[n].regs.xss = __KERNEL_SS;
    g_pcb_table[n].regs.esp = 0;// TODO:
    g_pcb_table[n].regs.eip = (uint32_t)function;
    g_pcb_table[n].regs.eflags = 0; // TODO:

    g_pcb_table[n].pid = pid;

    // g_pcb_table[n].p_name = "test";
}