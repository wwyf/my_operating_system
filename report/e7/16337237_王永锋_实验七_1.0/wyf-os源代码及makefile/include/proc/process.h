#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <type.h>
#include <const.h>
#include <mm/memory.h>

/* 用来保存CPU的上下文信息 */
// NOTICE:在push的时候，无论是普通寄存器还是段寄存器，push都是32位
typedef struct pt_regs{
    uint32_t ebx; // 0x00
    uint32_t ecx; // 0x04
    uint32_t edx; // 0x08
    uint32_t esi; // 0x0c
    uint32_t edi; // 0x10
    uint32_t ebp; // 0x14
    uint32_t eax; // 0x18
    uint32_t xds; // 0x1a
    uint32_t xes; // 0x1c
    uint32_t xfs; // 0x1e
    // 中断号
    uint32_t orig_eax; // 0x22
    // 返回地址信息
    uint32_t eip;
    uint32_t xcs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t xss;
}proc_regs_t;


/* 这就是PCB啦，进程控制块 */
typedef struct s_proc{
    proc_regs_t regs;
    uint32_t pid;
    uint32_t status; /* 0  */
    char p_name[16];
    void * kernel_stack;

    /* 该进程栈基地址 */
    uint32_t stack_base;

    /* 进程优先级 及时间片信息*/
    uint32_t remain_ticks;                 /* remained ticks */
    uint32_t priority;

    /* 进程的内存信息 */
    mm_struct_t *mm, *active_mm;

    /* 消息发送信息 */
    uint32_t p_recvfrom;
    uint32_t p_sendto;
    uint32_t p_flags;
    uint32_t has_int_msg;
    struct s_proc * q_sending;
    struct s_proc * next_sending;
    /* 消息内容 */
    message_t * p_msg;

    /* 父进程 */
    uint32_t p_parent;
    uint32_t exit_status;

}proc_task_struct_t;


// #define _PROC_SENDING 4
// #define _PROC_RECEIVING 3
#define _PROC_STOP 2
#define _PROC_SLEEP 1
#define _PROC_RUN 0
#define _PROC_EMPTY 99


void process_init();

void proc_sleep_myself();
void proc_wake_pid(int n);

void proc_init_a_task(uint32_t n, char * name, uint32_t pid, void * function, uint32_t priority);

#endif

