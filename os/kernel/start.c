#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/system_call.h"
#include "../include/fsystem.h"

extern FileDescriptor root[16];

u16 system_call[MAX_SYSTEM_CALL];

void (**my_program)();
void test_system_call();

int cstart(){
    // install_system_call(2, test_system_call);
    clean_screen();
    set_cursor(0);
    init_fsystem();

    fs_show_root_file_table();
    // fs_show_file_by_name("u_test.bin");
    // puti(fs_get_file_size("u_test.bin"));

    // 分配内存时需要注意！
    // *my_program = 0x2000;
    // fs_load_by_name("u_test.bin", *my_program);
    // (*my_program)();
    // u16 ptr;
    // read_memory(0x0100,0x0000, 1, &ptr);
    // puti(ptr);

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