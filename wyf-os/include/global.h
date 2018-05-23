#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_ 

#include <type.h>
#include <const.h>
#include <chr_drv/tty_drv.h>
#include <proc/process.h>
#include <mm/page.h>

/* 定义在tty_drv.c中 */
extern struct tty_struct g_tty_table[1];

// 页表
/* 定义在head.asm中 */
extern page_dir_entry_t g_page_dir[1024];

// 中断描述符表
/* 定义在head.asm中 */
extern desc_table_t g_idt_table;

// 全局描述符表
/* 定义在head.asm中 */
extern desc_table_t g_gdt_table;

/* 定义在process.h中 */
extern proc_task_struct_t g_pcb_table[MAX_PROCESS_NUM]; /// 进程控制块表。
extern proc_task_struct_t * g_cur_proc; /// 当前进程。
extern proc_regs_t * g_cur_proc_context_stack; /// 当前进程恢复上下文所用内核栈。


#endif