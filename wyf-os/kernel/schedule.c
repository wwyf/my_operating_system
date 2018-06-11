#include <type.h>
#include <const.h>
#include <global.h>
#include <common/debug.h>
#include <proc/process.h>


void proc_schedule(){
    if (g_cur_proc->remain_ticks){
        /* 时间片没有用完 */
        g_cur_proc->remain_ticks--;
        return ;
    }
    else {
        /* 时间片用完了 */

        /* 恢复时间片 */
        g_cur_proc->remain_ticks = g_cur_proc->priority;


        /* 不停寻找，直到找到可用进程 */
        int num = g_cur_proc - g_pcb_table;
        // FIXME:硬编码了当前进程数量
        int next_num = (num + 1)  % _PROC_NR_PROCS;
        /* 进程满足以下条件的话，不能调度
        1. 停止状态
        2. 睡眠状态，但是仍然被阻塞 */
        while (g_pcb_table[next_num].status == _PROC_STOP ||          (g_pcb_table[next_num].status == _PROC_SLEEP && 
            g_pcb_table[next_num].p_flags != 0) ){
            next_num = (next_num + 1)  % _PROC_NR_PROCS;
        }

        /* 找到处于RUN状态，或者可以run的进程后，修改当前进程 */
        g_cur_proc = &g_pcb_table[next_num];
        /* 可能本来就是run，或者水面但是不被阻塞了 */
        g_cur_proc->status = _PROC_RUN;
        g_cur_proc_context_stack = g_cur_proc->kernel_stack;
    }

    // com_printk(" g_cur_proc_context_stack %x", g_cur_proc_context_stack);
}
