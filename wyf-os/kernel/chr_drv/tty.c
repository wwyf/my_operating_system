#include <tty.h>
// 将检查队列是否为空的职责交给调用者
char _tty_queue_get(struct tty_queue * q){
	int cur_head = q->head;
	q->head = (q->head+1) % TTY_BUF_SIZE;
	return q->buf[cur_head];
}

void _tty_queue_put(struct tty_queue * q, char c){
	q->tail = (q->tail+1) % TTY_BUF_SIZE;
	q->buf[q->tail] = c;
}

int _tty_queue_is_full(struct tty_queue * q){
	return ( q->head == (q->tail+1 % TTY_BUF_SIZE ));
}

int _tty_queue_is_empty(struct tty_queue * q){
	return (q->head == q->tail);
}
