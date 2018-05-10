#include <console.h>
#include <global.h>
void test(){
    _sys_set_cursor(0);
    _sys_set_cursor(80);
    _sys_set_cursor(1920);
    con_write(&tty_table[0]);
    __asm__("int $0x80");
}