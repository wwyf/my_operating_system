#ifndef _TTY_DRV_H_
#define _TTY_DRV_H_

int tty_write(unsigned channel, char * buf, int nr);

void tty_init();

#endif