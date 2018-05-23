#ifndef _TTY_DRV_H_
#define _TTY_DRV_H_


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


// 将检查队列是否为空的职责交给调用者
char _tty_queue_get(struct tty_queue * q);

void _tty_queue_put(struct tty_queue * q, char c);

int _tty_queue_is_full(struct tty_queue * q);

int _tty_queue_is_empty(struct tty_queue * q);


int tty_write(unsigned channel, char * buf, int nr);

void tty_init();

#endif