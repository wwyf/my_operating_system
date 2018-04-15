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
    init_fsystem();
    clean_screen();
    set_cursor(0);
    // install_system_call(2, test_system_call);
    // while (1){}
    return 0;
}

void test_system_call(){
    int origin = get_cursor();
    set_cursor(1800);
    printf("test custom system call!!! ");
    set_cursor(origin);
}

