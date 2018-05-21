#ifndef _HEAD_H_
#define _HEAD_H_
/* 页表，中断表描述符，全局描述符 */
#include <type.h>

#define idt_table _idt
#define gdt_table _gdt


// 页表
extern uint32_t _pg_dir[1024];
// 中断描述符表
extern desc_table_t _idt;
// 全局描述符表
extern desc_table_t _gdt;

#endif
