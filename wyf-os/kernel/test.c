#include <console.h>
void test(){
    _sys_set_cursor(0);
    _sys_set_cursor(80);
    _sys_set_cursor(1920);
    __asm__("int $0x80");
}