#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"


u16 system_call[MAX_SYSTEM_CALL];
FileDescriptor root[16];

void test_system_call();

int cstart(){
    install_system_call(2, test_system_call);
    read_n_sector(37,1,0x1000,root);// 加载用户程序到指定地址处。
    clean_screen();
    set_cursor(0);
    putc(root[0].file_name1);
    putc(root[0].file_name2);
    puti(root[0].cluster_number);
    putc(root[1].file_name2);
    puti(root[1].cluster_number);
    putc(root[2].file_name3);
    putc(root[2].file_name4);
    puti(root[2].cluster_number);
    while (1){}
    return 0;
}

void test_system_call(){
    int origin = get_cursor();
    set_cursor(1800);
    printf("test custom system call!!! ");
    set_cursor(origin);
}