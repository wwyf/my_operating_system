#ifndef _STDIO_H_
#define _STDIO_H_

#include "type.h"

/* 已完成测试
在指定位置显示字符 */
void _put_char(char c, u16 cursor_index);

/* 已完成测试
设置光标位置 */
void set_cursor(u16 cursor_index);

/* 有bug
获取光标位置 */
u16 get_cursor();

/* 将字符放到光标处,光标向后退一格，
若光标已经处于行末，则自动进行回车换行
若光标已经处于屏幕末端最后一个字符，则进行滚屏 */
void puts(char c);

/* 格式化字符串，放到第一个参数所指的地址处,字符串后需加0 */
// void sprintf(char * , char * format , ...);

/* 将字符串输出到屏幕上 */
// void printf(char *, ...);

// void scanf(char *, ...);

#endif