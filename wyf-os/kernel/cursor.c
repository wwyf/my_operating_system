#include <basic.h>
#include <type.h>

void _sys_set_cursor(uint16_t cursor_index){
    // short cursor_index = 80 * row + column;     // 计算光标寄存器的值
    uint8_t low_eight = cursor_index; // 取低八位，高位被截断
    uint8_t high_eight = cursor_index >> 8; // 取高八位
    _basic_outb(0x3d4,0x0e);// 指定索引寄存器为光标寄存器的高八位
    _basic_outb(0x3d5, high_eight);// 将光标索引高八位写入端口
    _basic_outb(0x3d4,0x0f);// 指定索引寄存器为光标寄存器的低八位
    _basic_outb(0x3d5, low_eight);// 将光标索引低八位写入端口
    return ;
}

uint16_t _sys_get_cursor(){
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
