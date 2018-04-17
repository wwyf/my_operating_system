#include "../include/type.h"
#include "../include/stdio.h"
#include "../include/global.h"

void install_system_call(int system_call_number, void (*function)() ){
    system_call[system_call_number] = (u16) function;
    return ;
}



void test_system_call(){
    int origin = get_cursor();
    set_cursor(1800);
    printf("test custom system call!!! ");
    set_cursor(origin);
}

