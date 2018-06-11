#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_ 

#include <type.h>
#include <const.h>
#include <chr_drv/tty_drv.h>
#include <proc/process.h>
#include <mm/page.h>

/**
 * @brief 终端设备表，定义在tty_drv.c中
 * 
 */
extern tty_struct_t g_tty_table[1];

/**
 * @brief 页目录表，定义在head.asm中
 * 
 */
extern page_dir_entry_t g_page_dir[1024];

/**
 * @brief 中断描述符表
 * 
 * 中断描述符表，总共256项，定义在head.asm中 
 */
extern desc_table_t g_idt_table;

/**
 * @brief 全局描述符表
 * 
 * GDT，全局描述符表，定义在head.asm中
 */
extern desc_table_t g_gdt_table;

/**
 * @brief 进程控制块表
 * 
 * 定义在process.h中 
 */
extern proc_task_struct_t g_pcb_table[_PROC_NR_PROCS]; 

/**
 * @brief 当前进程指针
 * 
 * 
 * 该为指向当前进程的进程控制块的指针
 */
extern proc_task_struct_t * g_cur_proc;

/**
 * @brief 当前进程内核栈地址
 * 
 * 
 * 用于恢复上下文
 */
extern proc_regs_t * g_cur_proc_context_stack;


extern uint32_t  g_ticks;


#endif