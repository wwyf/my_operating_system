#include <intr/clock.h>
#include <basic.h>
#include <proc/schedule.h>
#include <common/common.h>

void irq0_clock_handler(){
    com_print("in the clock!!!");
    proc_schedule();
    _basic_outb(0x20, 0x20); // 发送EOI到主8529A
    _basic_outb(0xA0, 0x20); // 发送EOI到从8529A
}



void clock_init(){
    _basic_outb(0x43, 0x34); // 写控制字到控制字寄存器 
    _basic_outb(0x40, 0xff); // 中断时间设置 写计数器 0 的低字节 
    _basic_outb(0x40, 0xff); // 写计数器的高字节  
}