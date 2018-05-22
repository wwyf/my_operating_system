#include <stdarg.h>
#include <common/debug.h>
#include <chr_drv/tty_drv.h>

static char buf[1024];

int printk(char * fmt, ...){
    va_list args;
    int i;
    va_start(args, fmt);
    i = vsprintk(buf, fmt, args);
    tty_write(0, buf, i);
    return i;
}