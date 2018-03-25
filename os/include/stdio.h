#ifndef _STDIO_H_
#define _STDIO_H_

#include "type.h"

/* tested
在指定位置显示字符 */
void _put_char(char c, u16 cursor_index);

/* tested
设置光标位置 */
void set_cursor(u16 cursor_index);

/* tested
获取光标位置 */
u16 get_cursor();

/* 将字符放到光标处,光标向后退一格，
若光标已经处于行末，则自动进行回车换行
若光标已经处于屏幕末端最后一个字符，则进行滚屏 TODO:没有实现滚屏 */
void putc(char c);

/* tested
在光标处输出整数 */
void puti(u32 n);

/* tested
在光标处输出字符串 */
void puts(char * str, int size);

/* tested
必须保证字符串有足够空间，格式化字符串，放到第一个参数所指的地址处,字符串后需加0
暂时支持%d %c,  \n , \r */
void sprintf(char * , char * format , ...);

/* tested
将可变参数转变为 va_list a
va_list 实际上是一个参数数组
缓冲区大小定义在type.h中 */
void vprintf(char * format, va_list a);

/* tested
将字符串输出到屏幕上 */
void printf(char * format, ...);


/* tested
将指定的中断修改为自定义的中断，并设置中断处理例程的段地址和偏移量 */
void _install_interrupt_handler(u8 n, u16 segment_address, u16 entry_offset);

/* tested
将指定的中断修改为自定义的中断，并使用函数指针设置中断处理例程的地址 */
void install_interrupt_handler(u8 n, void (*interrupt_handler)());


/* TODO:
从键盘缓冲区获取一个键盘输入 */


/* TODO:
与键盘进行端口交互，获取键盘输入 */
void check_keyboard();

/* TODO:
从键盘缓冲区获得输入，并写到相应的地址处。*/
// void scanf(char *, ...);

#endif