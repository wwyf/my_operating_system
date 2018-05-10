#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <basic.h>
#include <type.h>
#include <tty.h>

void _console_init();

void _console_set_cursor(uint16_t cursor_index);

uint16_t _console_get_cursor();

void _console_write(struct tty_struct * tty);


#endif