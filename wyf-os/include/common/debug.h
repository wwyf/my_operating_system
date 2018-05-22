#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <common/stdarg.h>

void print(char * format, ...);

int printk( char * fmt, ...);

int vsprintk(char *buf, const char *fmt, va_list args);

#endif