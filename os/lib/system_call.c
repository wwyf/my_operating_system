#include "../include/type.h"

extern u16 system_call[MAX_SYSTEM_CALL];

void install_system_call(int system_call_number, void (*function)() ){
    system_call[system_call_number] = function;
    return ;
}