#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <basic.h>
#include <type.h>
#include <tty.h>

void con_write(struct tty_struct * tty);


void _sys_set_cursor(uint16_t cursor_index);
uint16_t _sys_get_cursor();

#endif