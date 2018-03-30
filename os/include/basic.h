#ifndef _BASIC_H_
#define _BASIC_H_

#include "type.h"

/* tested
往内存绝对地址写16位数据 */
void write_memory_word(u32 address, u16 data);

/* tested
往内存绝对地址写8位数据 */
void write_memory_byte(u32 address, u8 data);

/* tested
查看内存绝对地址处的内容 */
u16 read_memory_word(u32 address);

/* tested
将8位的数据写到指定端口 */
void write_port_byte(u16 port_number, u8 data);

/* tested
从指定端口中读出8位数据，并作为返回值返回 */
u8 read_port_byte(u16 port_number);

/* TODO:
取得段寄存器的值*/

/* 
检测键盘缓冲区，若有输入则返回1，若没有输入则返回0 */
u16 check_keyboard();
/* 
读入键盘缓冲区 */
u16 get_keyboard();

/* 
清屏 */
void clean_screen();

/* 
向下滚屏 */
void scroll_screen();

#endif