#ifndef _HEAD_H_
#define _HEAD_H_
/* 页表，中断表描述符，全局描述符 */
#include <type.h>


// 页表
extern uint32_t g_pg_dir[1024];
// 中断描述符表
extern desc_table_t g_idt_table;
// 全局描述符表
extern desc_table_t g_gdt_table;

#endif
