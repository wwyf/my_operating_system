#include <console.h>
#include <global.h>
#include <common/debug.h>
#include <tty_drv.h>
#include <common/string.h>
#include <protect.h>

/* 用来测试中断向量表的设置 */

void example_interrupt(){
    print("test in interrupt!!");
    print("test success!!!!!!");
}

void test2(){
    print("test default interrupt!");
    while(1){
        asm("int $81");
        asm("int $82");
    }
    print("testing interrupt!");
    set_intr_gate(0x79, example_interrupt);
    asm("int $0x79");
    print("return to kernel");
    while(1){}
}



// /* 用来测试字符设备的使用 */
void test1(){
    _console_set_cursor(0);
    _console_set_cursor(80);
    print("asdfasf!");
    printk("printk!!test!!printk%d", 11122);
    _console_set_cursor(1920);
    _tty_queue_put(&((&tty_table[0])->write_q), 'a');
    print("head: %d" ,tty_table[0].write_q.head);
    print("tail: %d" ,tty_table[0].write_q.tail);
    print("data: %c" ,tty_table[0].write_q.buf[tty_table[0].write_q.head]);
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
    tty_write(0, t, com_strlen(t));
    __asm__("int $0x80");
    // while(1){};
}