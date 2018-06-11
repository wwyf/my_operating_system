/* 这里存放的是tty字符设备 */
#include <chr_drv/console.h>
#include <chr_drv/tty_drv.h>

tty_struct_t g_tty_table[1] = {
    {
    _console_write,
    {0,0,0,""},
    }
};

/**
 * @brief 将buf缓冲区的内容写入第channel个tty设备
 * 
 * @param channel tty设备编号（一般是0）
 * @param buf 字符缓冲区
 * @param nr 字符缓冲区大小（缓冲区字符数）
 * @return int 写入的字符数
 */
int tty_write(uint32_t channel, char * buf, uint32_t nr)
{
	tty_struct_t * tty = channel + g_tty_table;
    uint32_t cur_char_index = 0;
    // TODO:终端号的范围
	if (channel>2 || nr<0) return -1;

	while (cur_char_index < nr) {
        // TODO:将缓冲区内的字符放入tty的缓冲区中，并将tty内没输出的字符给输出了。
        while(!_tty_queue_is_full(&tty->write_q) && cur_char_index < nr){
            _tty_queue_put(&tty->write_q, buf[cur_char_index]);
            cur_char_index++;
        }
        tty->write(tty);
	}
	return cur_char_index;
}



void tty_init(){
    _console_init();
}


/* 
tty 缓冲区队列处理函数
将检查队列是否为空的职责交给调用者
*/

/**
 * @brief 从队列里取一个字符（NOTICE:不会检查队列是否为空）
 * 
 * @param q 指向一个数组队列的指针
 * @return char 取得的字符
 */
char _tty_queue_get(tty_queue_t * q){
	int cur_head = q->head;
	q->head = (q->head+1) % TTY_BUF_SIZE;
	return q->buf[cur_head];
}

/**
 * @brief 将一个队列放入一个字符
 * 
 * @param q 指向一个数组队列的指针
 * @param c 要放入的字符
 */
void _tty_queue_put(tty_queue_t * q, char c){
	q->buf[q->tail] = c;
	q->tail = (q->tail+1) % TTY_BUF_SIZE;
}


/**
 * @brief 检查队列是否已满
 * 
 * @param q 指向数组队列的指针
 * @return int 返回1则满，否则为0，不满
 */
int _tty_queue_is_full(tty_queue_t * q){
	return ( q->head == (q->tail+1 % TTY_BUF_SIZE ));
}


/**
 * @brief 检查队列是否为空
 * 
 * @param q 指向数组队列的指针
 * @return int 返回1则空，否则为0，不空
 */
int _tty_queue_is_empty(tty_queue_t * q){
	return (q->head == q->tail);
}
