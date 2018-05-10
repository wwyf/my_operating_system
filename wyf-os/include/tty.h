#ifndef _TTY_H_
#define _TTY_H_

#include <basic.h>
#include <type.h>

#define TTY_BUF_SIZE 100
void _sys_set_cursor(uint16_t cursor_index);
uint16_t _sys_get_cursor();


struct tty_queue {
	unsigned long data;
	unsigned long head;
	unsigned long tail;
	// struct task_struct * proc_list;
	char buf[TTY_BUF_SIZE];
};


struct tty_struct {
	// struct termios termios;
	// int pgrp;
	// int stopped;
	void (*write)(struct tty_struct * tty);

	// struct tty_queue read_q;
	struct tty_queue write_q;
	// struct tty_queue secondary;
	};


// _tty_queue_get(char * c){

// }

#endif