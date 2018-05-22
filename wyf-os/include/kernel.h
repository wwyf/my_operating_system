#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stdarg.h>

int printk( char * fmt, ...);

int vsprintk(char *buf, const char *fmt, va_list args);

#endif
