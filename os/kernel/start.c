#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/global.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"
#include "../include/proc.h"

int cstart(){
    // 初始化文件系统
    init_fsystem();
    // 清屏
    clean_screen();
    // 将光标设置为0 TODO:
    set_cursor(0);


    // 加载文件到内存指定位置
    fs_load_by_name("test_a.bin", 0x2000, 0x0000);
    fs_load_by_name("test_b.bin", 0x3000, 0x0000);
    // fs_load_by_name("ouch.bin", 0x3000, 0x0000);
    // 初始化进程控制块
    init_process(0, 0x2000);
    init_process(1, 0x3000);

    // for (int i = 0; i < CUR_PROCESS_NUM ; i++){
    //     init_process(i, 0x2000 + i * 0x1000);
    // }
    // 初始化启动进程
    cur_process_index = 0;
    cur_process = (PCB_t*)(&process_table[cur_process_index]);

    // 初始化 系统调用表
    install_system_call(2, test_system_call);
    return 0;
}
