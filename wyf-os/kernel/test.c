#include <console.h>
#include <global.h>
#include <debug.h>
void test(){
    _sys_set_cursor(0);
    _sys_set_cursor(80);
    printf("asdfasf");
    _sys_set_cursor(1920);
    _tty_queue_put(&((&tty_table[0])->write_q), 'a');
    printf("head: %d" ,tty_table[0].write_q.head);
    printf("tail: %d" ,tty_table[0].write_q.tail);
    printf("data: %c" ,tty_table[0].write_q.buf[tty_table[0].write_q.head]);
    con_write(&tty_table[0]);
    _tty_queue_put(&(&tty_table[0])->write_q, 'x');
    _tty_queue_put(&(&tty_table[0])->write_q, 'a');
    _tty_queue_put(&(&tty_table[0])->write_q, 'c');
    _tty_queue_put(&(&tty_table[0])->write_q, 'd');
    _tty_queue_put(&(&tty_table[0])->write_q, 'e');
    _tty_queue_put(&(&tty_table[0])->write_q, 'b');
    _tty_queue_put(&(&tty_table[0])->write_q, 'i');
    con_write(&tty_table[0]);
    __asm__("int $0x80");
    while(1){};
}