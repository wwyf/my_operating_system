/* 这里存放的是tty字符设备 */
#include <console.h>
#include <tty.h>

struct tty_struct tty_table[1] = {
    {
    con_write,
    {0,0,0,"abcdefg"},
    }
};