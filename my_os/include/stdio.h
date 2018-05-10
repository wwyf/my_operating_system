#ifndef _STDIO_H_
#define _STDIO_H_


#include "type.h"
#define EXTERN extern

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
void _putc(char c);// 不改变光标

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


// /* tested
// 将指定的中断修改为自定义的中断，并设置中断处理例程的段地址和偏移量 */
// void _install_interrupt_handler(u8 n, u16 segment_address, u16 entry_offset);

// /* tested
// 定义一个 */
// void install_interrupt_handler(u8 n, void (*interrupt_handler)());


/* 
判断一个字符是否是数字 */

int isprint (int ch);
int ispunct (int ch);
int isalnum (int ch);
int isalpha (int ch);
int isblank (int ch);
int iscntrl (int ch);
int isdigit (int ch);


/* 
读取指定逻辑扇区号的扇区到指定内存地址处 */
void read_n_sector(u16 sector_code, u16 number,  u16 segment, u16 offset);

/* 
读取内存中指定地址起n字节的内容到指定指针处 */
void read_memory(u16 segment, u16 offset, u16 word_size, u16 * ptr);

#endif