#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <common/stdarg.h>

void com_print(char * format, ...);

int com_printk( char * fmt, ...);

int com_vsprintk(char *buf, const char *fmt, va_list args);

int com_sprintk(char * dest, char * fmt, ...);

#endif