#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/global.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"
#include "../include/proc.h"

void schedule_process(){
    // printf("wowwow");
    for (int i = 0; i < 10000000; i++);
    cur_process_index = (cur_process_index + 1) % cur_process_num;
    cur_process = &process_table[cur_process_index];
}


void init_process(int process_index, u16 segment){
    PCB_t* this_process = &process_table[process_index];
    this_process->regs.ss = segment;
    this_process->regs.sp = 0x5000;
    this_process->regs.ds = segment;
    this_process->regs.es = segment;
    this_process->regs.edi = 0;
    this_process->regs.esi = 0;
    this_process->regs.ebp = 0x5000;
    this_process->regs.kernel_esp = 0;
    this_process->regs.ebx = 0;
    this_process->regs.edx = 0;
    this_process->regs.ecx = 0;
    this_process->regs.eax = 0;
    this_process->regs.ip = 0x0000;
    this_process->regs.cs = segment;
    this_process->regs.flags = 0x1202;
    this_process->pid = process_index;
    // this_process->p_name = 0;TODO:实现strcpy函数再说。
}
