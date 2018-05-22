#ifndef _GLOBAL_H_ 
#define _GLOBAL_H_ 

#include <type.h>
#include <chr_drv/tty_drv.h>

/* 定义在tty_drv.c中 */
extern struct tty_struct g_tty_table[1];

// 页表
/* 定义在head.asm中 */
extern uint32_t g_pg_dir[1024];

// 中断描述符表
/* 定义在head.asm中 */
extern desc_table_t g_idt_table;

// 全局描述符表
/* 定义在head.asm中 */
extern desc_table_t g_gdt_table;


#endif