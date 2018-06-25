#ifndef _CONST_H_
#define _CONST_H_

#define NULL ((void *)0)

/* 本头文件放的是一些系统中用到的常量 */

#define __KERNEL_CS 0x10
#define __KERNEL_DS 0x08
#define __KERNEL_FS 0x08
#define __KERNEL_ES 0x08
#define __KERNEL_SS 0x08



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


/* 消息机制中的一些常量 */
/**
 * @enum msgtype
 * @brief MESSAGE types
 */
enum msgtype {
	/* 
	 * when hard interrupt occurs, a msg (with type==HARD_INT) will
	 * be sent to some tasks
	 */
	HARD_INT = 1,

	/* SYS task */
	GET_TICKS, GET_PID, GET_RTC_TIME,

	/* semaphore */
	SEMAPHORE,

	/* FS */
	OPEN, CLOSE, READ, WRITE, LSEEK, STAT, UNLINK,

	/* FS & TTY */
	SUSPEND_PROC, RESUME_PROC,

	/* MM */
	EXEC, WAIT,

	/* FS & MM */
	FORK, EXIT,

	/* TTY, SYS, FS, MM, etc */
	SYSCALL_RET,

	/* message type for drivers */
	DEV_OPEN = 1001,
	DEV_CLOSE,
	DEV_READ,
	DEV_WRITE,
	DEV_IOCTL
};

/* macros for messages */
#define	FD		u.m3.m3i1
#define	PATHNAME	u.m3.m3p1
/* for semaphore */
#define	FLAGS		u.m3.m3i1
#define VALUE u.m3.m3i2
#define	NAME_LEN	u.m3.m3i2
#define	CNT		u.m3.m3i2
#define	REQUEST		u.m3.m3i2
#define	PROC_NR		u.m3.m3i3
#define	DEVICE		u.m3.m3i4
#define	POSITION	u.m3.m3l1
#define	BUF		u.m3.m3p2
#define	OFFSET		u.m3.m3i2
#define	WHENCE		u.m3.m3i3

#define	PID		u.m3.m3i2
#define	STATUS		u.m3.m3i1
#define	RETVAL		u.m3.m3i1


/* the assert macro */
#define ASSERT
#ifdef ASSERT
void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp)  if (exp) ; \
        else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif

/* magic chars used by `printx' */
#define MAG_CH_PANIC	'\002'
#define MAG_CH_ASSERT	'\003'


/* Process */
#define SENDING   0x02	/* set when proc trying to send */
#define RECEIVING 0x04	/* set when proc trying to recv */
#define WAITING   0x08	/* set when proc waiting for the child to terminate */
#define HANGING   0x10	/* set when proc exits without being waited by parent */
#define FREE_SLOT 0x20	/* set when proc table entry is not used
						* (ok to allocated to a new process)
						*/

/* semaphore use msg.FLAGS */
#define SEM_GETSEM 0x01
#define SEM_FREESEM 0x02
#define SEM_P 0x03
#define SEM_V 0x04

/* ipc */
#define SEND		1
#define RECEIVE		2
#define BOTH		3	/* BOTH = (SEND | RECEIVE) */

/* Number of tasks & processes */
#define _PROC_NR_TASKS		5
#define _PROC_NR_PROCS		10
#define _PROC_NUM (_PROC_NR_PROCS + _PROC_NR_TASKS + 5)



/* tasks */
/* 注意 TASK_XXX 的定义要与 global.c 中对应 */
#define INVALID_DRIVER	-20
#define INTERRUPT	-10
#define TASK_TTY	0
#define TASK_SYS	1
#define TASK_HD		2
#define TASK_FS		3
#define TASK_MM		4
#define TASK_INIT		5
#define ANY		(_PROC_NR_PROCS + _PROC_NR_TASKS + 10)
#define NO_TASK		(_PROC_NR_PROCS + _PROC_NR_TASKS + 20)

/* descriptor indices in LDT */
#define INDEX_LDT_C             0
#define INDEX_LDT_RW            1



#endif