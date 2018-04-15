#include "../include/type.h"
#include "../include/stdio.h"
#include "../include/global.h"

void install_system_call(int system_call_number, void (*function)() ){
    system_call[system_call_number] = (u16) function;
    return ;
}