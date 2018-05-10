#ifndef _HEAD_H
#define _HEAD_H
/* 页表，中断表描述符，全局描述符 */
#include <type.h>

typedef struct desc_struct {
	uint32_t a,b;
} desc_table[256];

extern uint32_t _pg_dir[1024];
extern desc_table _idt,_gdt;

#endif
