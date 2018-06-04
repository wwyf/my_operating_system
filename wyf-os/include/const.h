#ifndef _CONST_H_
#define _CONST_H_

/* 本头文件放的是一些系统中用到的常量 */

#define __KERNEL_CS 0x10
#define __KERNEL_DS 0x08
#define __KERNEL_FS 0x08
#define __KERNEL_ES 0x08
#define __KERNEL_SS 0x08

#define MAX_PROCESS_NUM 4


/* 8259A interrupt controller ports. */
/* 8259A 中断控制器，相关端口 */
#define	INT_M_CTL	0x20	/* I/O port for interrupt controller         <Master> */
#define	INT_M_CTLMASK	0x21	/* setting bits in this port disables ints   <Master> */
#define	INT_S_CTL	0xA0	/* I/O port for second interrupt controller  <Slave>  */
#define	INT_S_CTLMASK	0xA1	/* setting bits in this port disables ints   <Slave>  */


/* Hardware interrupts */
/* 硬件IRQ线中断号 */
#define	NR_IRQ		16	/* Number of IRQs */
#define	CLOCK_IRQ	0
#define	KEYBOARD_IRQ	1
#define	CASCADE_IRQ	2	/* cascade enable for 2nd AT controller */
#define	ETHER_IRQ	3	/* default ethernet interrupt vector */
#define	SECONDARY_IRQ	3	/* RS232 interrupt vector for port 2 */
#define	RS232_IRQ	4	/* RS232 interrupt vector for port 1 */
#define	XT_WINI_IRQ	5	/* xt winchester */
#define	FLOPPY_IRQ	6	/* floppy disk */
#define	PRINTER_IRQ	7
#define	AT_WINI_IRQ	14	/* at winchester */

#define SECTOR_SIZE 512

#endif