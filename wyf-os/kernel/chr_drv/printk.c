#include <common/stdarg.h>
#include <common/common.h>
#include <chr_drv/tty_drv.h>

static char buf[1024];

int com_printk(char * fmt, ...){
    va_list args;
    int i;
    va_start(args, fmt);
    i = com_vsprintk(buf, fmt, args);
    tty_write(0, buf, i);
    return i;
}

int com_sprintk(char * dest, char * fmt, ...){
    va_list args;
    int i;
    va_start(args, fmt);
    i = com_vsprintk(dest, fmt, args);
    return i;
}