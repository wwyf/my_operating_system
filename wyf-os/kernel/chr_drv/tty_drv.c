/* 这里存放的是tty字符设备 */
#include <console.h>
#include <tty.h>

struct tty_struct tty_table[1] = {
    {
    con_write,
    {0,0,0,""},
    }
};



// 将该缓冲区内容写入第channel个tty设备
// 字符设备号，缓冲区，缓冲区大小
int tty_write(unsigned channel, char * buf, int nr)
{
	struct tty_struct * tty = channel + tty_table;
    int cur_char_index = 0;
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
