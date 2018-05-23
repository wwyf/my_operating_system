#include <intr/clock.h>
#include <basic.h>
#include <proc/schedule.h>
#include <common/common.h>

#define INT_M_CTL 0x20	// I/O port for interrupt controller         <Master>
#define INT_M_CTLMASK 0x21	// setting bits in this port disables ints   <Master>
#define INT_S_CTL 0xA0	// I/O port for second interrupt controller  <Slave>
#define INT_S_CTLMASK 0xA1	// setting bits in this port disables ints   <Slave>
#define EOI 0x20


void irq0_clock_handler(){

    // _basic_cli();
    // uint8_t master_port_value = _basic_inb(INT_M_CTLMASK);
    // _basic_outb(INT_M_CTLMASK, master_port_value | 1);//不允许再发生时钟中断
    // _basic_sti();


    proc_schedule();

    // _basic_cli();
    // uint8_t master_port_value =_basic_inb(INT_M_CTLMASK);
    // _basic_outb(INT_M_CTLMASK, master_port_value | 0xFE); // 又允许时钟中断发生
    // _basic_sti();
    _basic_outb(INT_M_CTL, EOI); // 发送EOI
}

void clock_init(){
    _basic_outb(0x43, 0x34); // 写控制字到控制字寄存器 
    _basic_outb(0x40, 0xff); // 中断时间设置 写计数器 0 的低字节 
    _basic_outb(0x40, 0xff); // 写计数器的高字节  
}


