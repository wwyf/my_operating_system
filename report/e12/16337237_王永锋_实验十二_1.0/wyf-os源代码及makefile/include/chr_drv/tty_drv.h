#ifndef _TTY_DRV_H_
#define _TTY_DRV_H_


#include <basic.h>
#include <type.h>

#define TTY_BUF_SIZE 100
void _sys_set_cursor(uint16_t cursor_index);
uint16_t _sys_get_cursor();


typedef struct tty_queue {
	uint32_t data;
	uint32_t head;
	uint32_t tail;
	// struct task_struct * proc_list;
	char buf[TTY_BUF_SIZE];
}tty_queue_t;


typedef struct tty_struct{
	// struct termios termios;
	// int pgrp;
	// int stopped;
	void (*write)(struct tty_struct * tty);

	// tty_queue_t read_q;
	tty_queue_t write_q;
	// tty_queue_t secondary;
}tty_struct_t;


// 将检查队列是否为空的职责交给调用者
char _tty_queue_get(tty_queue_t * q);

void _tty_queue_put(tty_queue_t * q, char c);

int _tty_queue_is_full(tty_queue_t * q);

int _tty_queue_is_empty(tty_queue_t * q);


int tty_write(uint32_t channel, char * buf, uint32_t nr);

void tty_init();

#endif