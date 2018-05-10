#include <console.h>
#include <global.h>
#include <debug.h>
#include <tty_drv.h>
#include <kernel.h>
#include <string.h>

void test1(){
    _console_set_cursor(0);
    _console_set_cursor(80);
    printf("asdfasf!");
    printk("printk!!test!!printk%d", 11122);
    _console_set_cursor(1920);
    _tty_queue_put(&((&tty_table[0])->write_q), 'a');
    printf("head: %d" ,tty_table[0].write_q.head);
    printf("tail: %d" ,tty_table[0].write_q.tail);
    printf("data: %c" ,tty_table[0].write_q.buf[tty_table[0].write_q.head]);
    _console_write(&tty_table[0]);
    _tty_queue_put(&(&tty_table[0])->write_q, 'x');
    _tty_queue_put(&(&tty_table[0])->write_q, 'a');
    _tty_queue_put(&(&tty_table[0])->write_q, 'c');
    _tty_queue_put(&(&tty_table[0])->write_q, 'd');
    _tty_queue_put(&(&tty_table[0])->write_q, 'e');
    _tty_queue_put(&(&tty_table[0])->write_q, 'b');
    _tty_queue_put(&(&tty_table[0])->write_q, 'i');
    _console_write(&tty_table[0]);
    char t[16] = "!!testtesttest";
    tty_write(0, t, strlen(t));
    __asm__("int $0x80");
    while(1){};
}