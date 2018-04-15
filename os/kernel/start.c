#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/global.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"

void (**my_program)();
void test_system_call();
int a;
int cstart(){
    install_system_call(2, test_system_call);
    clean_screen();
    set_cursor(0);

    a = get_cursor();

    set_cursor(0);
    set_cursor(1);
    set_cursor(2);
    set_cursor(3);
    set_cursor(4);
    set_cursor(345);


    // while (1){}


    init_fsystem();

    fs_show_root_file_table();

    // 分配内存时需要注意！
    // *my_program = 0x2000;
    // fs_load_by_name("stone1.bin", *my_program);
    // (*my_program)();

    return 0;
}

void test_system_call(){
    int origin = get_cursor();
    set_cursor(1800);
    printf("test custom system call!!! ");
    set_cursor(origin);
}

