#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <common/stdarg.h>
#include <type.h>

void com_print(char * format, ...);

int com_printk( char * fmt, ...);

int com_vsprintk(char *buf, const char *fmt, va_list args);

int com_sprintk(char * dest, char * fmt, ...);

PUBLIC void assertion_failure(char *exp, char *file, char *base_file, int line);

#endif