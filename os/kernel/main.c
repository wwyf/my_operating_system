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
    read_n_sector(1,1,0x0100,0x0000);// 读取fat表到指定地址处。
    clean_screen();
    set_cursor(0);

    u16 ptr;
    read_memory(0x0100,0x0000, 1, &ptr);
    puti(ptr);

    while (1){}
    return 0;
}

void test_system_call(){
    int origin = get_cursor();
    set_cursor(1800);
    printf("test custom system call!!! ");
    set_cursor(origin);
}

// void load_bin_file_by_name(){
//     Message msg;

//     char * file_name = (char *)
// }