#include <type.h>
#include <global.h>
#include <common/debug.h>
#include <proc/process.h>


void proc_schedule(){
    int num = g_cur_proc - g_pcb_table;
    // FIXME:硬编码了当前进程数量
    int next_num = (num + 1)  % 2;
    /* 只有处于RUN状态的进程才可以运行 */
    while (g_pcb_table[next_num].status == _PROC_SLEEP){
        next_num = (num + 1)  % 2;
    }
    g_cur_proc = &g_pcb_table[next_num];
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    // com_printk(" g_cur_proc_context_stack %x", g_cur_proc_context_stack);
}
