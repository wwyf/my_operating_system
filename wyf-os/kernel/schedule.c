#include <type.h>
#include <const.h>
#include <global.h>
#include <common/debug.h>
#include <proc/process.h>


PRIVATE void _switch_to(){
    /* 不停寻找，直到找到可用进程 */
    int num = g_cur_proc - g_pcb_table;
    // FIXME:硬编码了当前进程数量
    int proc_num = _PROC_NUM;
    int next_num = (num + 1)  % proc_num;
    /* 进程满足以下条件的话，不能调度
    1. 进程控制块为空
    1. 停止状态
    2. 睡眠状态，但是仍然被阻塞 */
    /* 对于能够运行的状态，进行取反 */
    while (!( g_pcb_table[next_num].status == _PROC_RUN || 
            (g_pcb_table[next_num].status != _PROC_EMPTY && g_pcb_table[next_num].p_flags == 0)
            )){
    // while (g_pcb_table[next_num].status == _PROC_STOP ||
    //         g_pcb_table[next_num].status == _PROC_EMPTY ||
    //             (g_pcb_table[next_num].status == _PROC_SLEEP && 
    //             g_pcb_table[next_num].p_flags != 0) ){
        // FIXME:硬编码了当前进程数量
        next_num = (next_num + 1); 
        
        next_num = next_num % proc_num;
        /* NOTICE:很奇怪,如果我向下面那样写就会错 */
        // next_num = (next_num + 1)  % _PROC_NUM;
    }
    /* 找到处于RUN状态，或者可以run的进程后，修改当前进程 */
    g_cur_proc = &g_pcb_table[next_num];
    /* 可能本来就是run，或者水面但是不被阻塞了 */
    g_cur_proc->status = _PROC_RUN;
    g_cur_proc_context_stack = g_cur_proc->kernel_stack;
}

/**
 * @brief 进行进程调度
 * 
 * 1. 先看当前进程是否被阻塞
 *  1. 如果被阻塞，就切换
 * 2. 再看时间片信息，如果时间片没用完，就不管，否则就切换
 * 
 */
void proc_schedule(){
    /* 如果当前进程被阻塞，就切换 */
    if (g_cur_proc->status == _PROC_STOP || 
        (g_cur_proc->status == _PROC_SLEEP && g_cur_proc->p_flags != 0)){
            _switch_to();
    }
    else if (g_cur_proc->remain_ticks){
        /* 时间片没有用完 */
        g_cur_proc->remain_ticks--;
        return ;
    }
    else if(!g_cur_proc->remain_ticks){
        /* 时间片用完了 */
        /* 恢复时间片 */
        g_cur_proc->remain_ticks = g_cur_proc->priority;
        _switch_to();
    }

    // com_printk(" g_cur_proc_context_stack %x", g_cur_proc_context_stack);
}

