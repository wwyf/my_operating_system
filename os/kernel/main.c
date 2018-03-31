#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/system_call.h"


u16 system_call[MAX_SYSTEM_CALL];

void test_system_call();

int cstart(){
    install_system_call(2, test_system_call);
    return 0;
}

void test_system_call(){
    int origin = get_cursor();
    set_cursor(1800);
    printf("test custom system call!!! ");
    set_cursor(origin);
}