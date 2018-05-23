#include <global.h>
#include <common/debug.h>
#include <common/string.h>
#include <protect/protect.h>
#include <chr_drv/console.h>
#include <chr_drv/tty_drv.h>


void _test1();
void _test2();
void _test3();


void main_test(){
    // _test1();
    // _test2();
    _test3();
}




/***********************************************/

/* 测试中断能够做到正确的保存环境和恢复环境。 */
void _test3(){
    while(1){
        asm("int $40");
        asm("int $48");
        // while(1){}
    }
}

/***********************************************/
/* 用于测试自定义中断的设置 */

void example_interrupt(){
    com_print("test in interrupt!!");
    com_print("test success!!!!!!");
}

void _test2(){
    com_print("test default interrupt!");
    com_print("testing interrupt!");
    set_intr_gate(0x79, example_interrupt);
    asm("int $0x79");
    com_print("return to kernel");
    while(1){}
}


/***********************************************/

// /* 用来测试字符设备的使用 */
void _test1(){
    _console_set_cursor(0);
    _console_set_cursor(80);
    com_print("asdfasf!");
    com_printk("com_printk!!test!!com_printk%d", 11122);
    _console_set_cursor(1920);
    _tty_queue_put(&((&g_tty_table[0])->write_q), 'a');
    com_print("head: %d" ,g_tty_table[0].write_q.head);
    com_print("tail: %d" ,g_tty_table[0].write_q.tail);
    com_print("data: %c" ,g_tty_table[0].write_q.buf[g_tty_table[0].write_q.head]);
    _console_write(&g_tty_table[0]);
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'x');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'a');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'c');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'd');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'e');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'b');
    _tty_queue_put(&(&g_tty_table[0])->write_q, 'i');
    _console_write(&g_tty_table[0]);
    char t[16] = "!!testtesttest";
    tty_write(0, t, com_strlen(t));
    __asm__("int $0x80");
    char s[30];
    com_sprintk(s, "test %d%s", 2342,"sdfs");
    com_printk(s);
    // while(1){};
}


