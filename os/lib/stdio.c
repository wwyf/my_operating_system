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
    u16 row = cursor_index / 80;
    u16 col = cursor_index % 80;
    switch (c) {
        case '\n':
            set_cursor((row+1)*80+col); // 回车，移到下一行
            break;
        case '\r':
            set_cursor(row*80);    // 移到本行开头处
            break;
        default:
            _put_char(c, cursor_index);
            set_cursor(cursor_index+1);
            break;
    }
    return ;
}

void puti(u32 n){
    int len = 0;
    int num[INT_LENGTH];  // 临时存放各位数，从低位放到高位，之后倒序输出即可
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

void puts(char * str, int size){
    int i = 0;
    while (i < size){
        putc(str[i]);
        i++;
    }
    return ;
}



void sprintf(char * dest , char * format, ...){
    int arg_num = 0;
    int src_index = 0;   // 源字符串索引，总是指向未读的一位
    int des_index = 0;   // 目标字符串索引，总是指向未写的一位。
    int* arg_addr = &format+1; // 边长参数第一个参数的地址。
    while (format[src_index] != 0){
        if (format[src_index] == '%'){
            src_index++;
            switch(format[src_index]){
                case 'c':{
                    dest[des_index++] =*(arg_addr + arg_num++);
                    src_index++;
                    break;
                }
                case 'd':{
                    int n = *(arg_addr + arg_num++);
                    int len = 0; 
                    int num[INT_LENGTH];
                    while (n != 0){
                        num[len] = n % 10;
                        n = n / 10;
                        len++;
                    }
                    for (int i = len-1; i >= 0; i--){
                        dest[des_index++] = num[i]+'0';
                    }
                    src_index++;
                    break;
                }
                case 's':{
                    char * str = *(arg_addr + arg_num++);
                    //TODO: 先写一个strlen（）
                    u32 str_len = strlen(str);
                    u32 i = 0;
                    while (i < str_len){
                        dest[des_index++] = str[i++];
                    }
                    src_index++;
                    break;
                }
                default :
                    src_index++;
                    break;
            }
        }
        else if (format[src_index] == '\\') {
            switch(format[src_index]){
                src_index++;
                switch(format[src_index]){
                    case 'n':
                        dest[des_index++] = 0x0a;
                        src_index++;
                        break;
                    case 'r':
                        dest[des_index++] = 0x0d;
                        src_index++;
                        break;
                    default:
                    src_index++;
                        break;
                }
            }
        }
        else {
            dest[des_index++] = format[src_index++];
        }
    }
    dest[des_index] = 0;
    return ;
}
