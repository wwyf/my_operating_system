#ifndef _PROTECT_H_
#define _PROTECT_H_

#include <type.h>

/**
 * @brief 作为底层函数，将相应的门设置到中断向量表中。
 * 
 * @param gate 中断向量号
 * @param type  中断类型（中断门，任务门，陷阱门？）
 * @param addr 中断处理例程函数指针
 * @param seg  段地址，一般为__KERNEL_CS,可见const.h
 */
void _set_gate(uint32_t gate, uint32_t type, void * addr, uint8_t seg);


/**
 * @brief  将参数打包成一个门描述符，a为低地址,b为高地址 
 * 
 * @param a 
 * @param b 
 * @param base 
 * @param seg 
 * @param type 
 * @param flags 
 */
void _pack_gate( uint32_t * a, uint32_t * b, uint32_t base, uint8_t seg, uint8_t type, uint8_t flags);

/**
 * @brief 写中断向量表
 * 
 * @param dt 中断向量表首地址 一般为 idt_table 这个全局变量
 * @param entry 中断向量号 
 * @param entry_low  中断门描述符低字节
 * @param entry_high 中断门描述符高位字节
 */
void _write_idt_entry(desc_struct_t * dt, int32_t entry, uint32_t entry_low, uint32_t entry_high);

/* 陷阱门 */
void set_trap_gate(uint32_t vector, void * addr);

/* 任务门 */
void set_task_gate(uint32_t vector, void * addr);

/* 系统中断 */
void set_system_intr_gate(uint32_t vector, void * addr);

/**
 * @brief Set the intr gate object
 * 
 * @param vector 中断向量号
 * @param addr 中断处理例程函数指针
 */
void set_intr_gate(uint32_t vector, void * addr);



#endif
