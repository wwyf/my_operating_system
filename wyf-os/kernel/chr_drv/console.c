#include <basic.h>
#include <type.h>
#include <common/string.h>
#include <chr_drv/tty_drv.h>

// 每行字节数
static uint16_t video_size_row;
// 列数
static uint16_t video_num_columns;
// 行数
static uint16_t video_num_lines;
static char * video_mem_start;
static char * video_mem_end;
// 用来擦除字符的字符
static uint8_t vodeo_erase_char;

// 光标所在位置。行，列
static uint16_t x,y;

void _console_init(){
    video_size_row = 160;
    video_num_columns = 80;
    video_num_lines = 25;
    video_mem_start = (char *)0xB8000;
    x = 0;
    y = 0;
    // video_mem_end = TODO:
}

/**
 * @brief 设置本控制台的光标位置
 * 
 * @param cursor_index 光标位置索引（0-1999）
 */
void _console_set_cursor(uint16_t cursor_index){
    // short cursor_index = 80 * row + column;     // 计算光标寄存器的值
    uint8_t low_eight = cursor_index; // 取低八位，高位被截断
    uint8_t high_eight = cursor_index >> 8; // 取高八位
    _basic_outb(0x3d4,0x0e);// 指定索引寄存器为光标寄存器的高八位
    _basic_outb(0x3d5, high_eight);// 将光标索引高八位写入端口
    _basic_outb(0x3d4,0x0f);// 指定索引寄存器为光标寄存器的低八位
    _basic_outb(0x3d5, low_eight);// 将光标索引低八位写入端口
    return ;
}


/**
 * @brief 获取光标位置索引
 * 
 * @return uint16_t 
 */
uint16_t _console_get_cursor(){
    uint8_t low_eight = 0, high_eight = 0;
    uint16_t cursor_index = 0;
    _basic_outb(0x3d4,0x0e);// 指定索引寄存器为光标寄存器的高八位
    high_eight = _basic_inb(0x3d5);// 将光标索引高八位写入端口
    _basic_outb(0x3d4,0x0f);// 指定索引寄存器为光标寄存器的低八位
    low_eight = _basic_inb(0x3d5);// 将光标索引低八位写入端口
    cursor_index = high_eight;
    cursor_index = (cursor_index << 8) + low_eight;
    return cursor_index;
}


/**
 * @brief 将控制台对应的输入缓冲队列的内容写入该控制台对应的显示区域中。
 * 
 * @param tty 需要输出的控制台结构体
 */
void _console_write(struct tty_struct * tty){
    /* 向终端设备写入字符并显示 */
    // TODO:光标没有去控制。
    // 需要输出的字符个数
    int current_num_char = com_strlen(tty->write_q.buf);
    // 开始输出的内存地址
    char * display_ptr =(char *)video_mem_start + x*video_size_row + y*2;
    while(!_tty_queue_is_empty(&tty->write_q)){
        char c = _tty_queue_get(&tty->write_q);
        display_ptr = (char *)(video_mem_start + x*video_size_row + y*2);
        *display_ptr = c;
        // 颜色
        *(display_ptr+1) = 0x0007;
        y++;
        if (y >= video_num_columns){
            y = 0;
            x++;
        }
        // TODO:x行数超出限制需要滚屏
    }
}