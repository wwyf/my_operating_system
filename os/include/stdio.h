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

/*
在光标处输出字符串,字符串以0结尾 */
void puts(char * str, int size);

/* 格式化字符串，放到第一个参数所指的地址处,字符串后需加0 */
// void sprintf(char * , char * format , ...);

/* 将字符串输出到屏幕上 */
// void printf(char *, ...);

// void scanf(char *, ...);

#endif