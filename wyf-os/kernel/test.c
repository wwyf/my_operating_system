#include <console.h>
#include <global.h>
void test(){
    _sys_set_cursor(0);
    _sys_set_cursor(80);
    _sys_set_cursor(1920);
    _tty_queue_put(&(&tty_table[0])->write_q, 'a');
    con_write(&tty_table[0]);
    _tty_queue_put(&(&tty_table[0])->write_q, 'x');
    con_write(&tty_table[0]);
    __asm__("int $0x80");
}