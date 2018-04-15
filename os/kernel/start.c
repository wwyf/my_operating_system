#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/global.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"
#include "../include/proc.h"

void test_system_call();
int cstart(){
    // 初始化文件系统
    init_fsystem();
    // 清屏
    clean_screen();
    // 将光标设置为0 TODO:
    set_cursor(0);
    // install_system_call(2, test_system_call);
    fs_load_by_name("test_a.bin", 0x5000);
    // while (1){}
    init_process(0);


    return 0;
}

void init_process(int process_index){
    PCB_t* this_process = &process_block[process_index];
    u16 segment = 0x1500;
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
};

// void test_system_call(){
//     int origin = get_cursor();
//     set_cursor(1800);
//     printf("test custom system call!!! ");
//     set_cursor(origin);
// }

