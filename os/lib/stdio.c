#include "../include/basic.h"
#include "../include/type.h"

void _put_char(char c, u16 cursor_index){
    int _address = 0xb8000;
    int _offset = cursor_index * 2;
    write_memory_byte(_address+_offset, c);
    return ;
}

void set_cursor(u16 cursor_index){
    // short cursor_index = 80 * row + column;     // 计算光标寄存器的值
    u8 low_eight = cursor_index; // 取低八位，高位被截断
    u8 high_eight = cursor_index >> 8; // 取高八位
    write_port_byte(0x3d4,0x0e);// 指定索引寄存器为光标寄存器的高八位
    write_port_byte(0x3d5, high_eight);// 将光标索引高八位写入端口
    write_port_byte(0x3d4,0x0f);// 指定索引寄存器为光标寄存器的低八位
    write_port_byte(0x3d5, low_eight);// 将光标索引低八位写入端口
    return ;
}

u16 get_cursor(){
    u8 low_eight = 0, high_eight = 0;
    u16 cursor_index = 0;
    write_port_byte(0x3d4,0x0e);// 指定索引寄存器为光标寄存器的高八位
    high_eight = read_port_byte(0x3d5);// 将光标索引高八位写入端口
    write_port_byte(0x3d4,0x0f);// 指定索引寄存器为光标寄存器的低八位
    low_eight = read_port_byte(0x3d5);// 将光标索引低八位写入端口
    cursor_index = high_eight;
    cursor_index = (cursor_index << 8) + low_eight;
    return cursor_index;
}

void putc(char c){
    u16 cursor_index = get_cursor();
    _put_char(c, cursor_index);
    set_cursor(cursor_index+1);
    return ;
}

void puti(u32 n){
    int len = 0;
    int num[10];
    while (n != 0){
        num[len] = n % 10;
        n = n / 10;
        len++;
    }
    for (int i = len-1; i >= 0; i--){
        putc(num[i]+'0');
    }
    return ;
}