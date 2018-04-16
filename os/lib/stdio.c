#include "../include/basic.h"
#include "../include/type.h"
#include "../include/string.h"

void _put_char(char c, u16 cursor_index){
    int _address = 0xb8000;
    int _offset = cursor_index * 2;
    write_memory_byte(_address+_offset, c);
    write_memory_byte(_address+_offset+1, 0x1f);
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
    if (cursor_index >= 1920){
        scroll_screen();
        cursor_index = 1840;
    }
    switch (c) {
        case '\n':
            set_cursor((row+1)*80); // 回车，移到下一行
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

void _putc(char c){
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
            // set_cursor(cursor_index+1);
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
                    char * str = *(char **)(arg_addr + arg_num++);
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


void vprintf(char * format, va_list va){
    char buf[BUF_LENGTH];
    int arg_num = 0;
    int src_index = 0;   // 源字符串索引，总是指向未读的一位
    int des_index = 0;   // 目标字符串索引，总是指向未写的一位。
    int* arg_addr = va; // 边长参数第一个参数的地址。
    while (format[src_index] != 0){
        if (format[src_index] == '%'){
            src_index++;
            switch(format[src_index]){
                case 'c':{
                    buf[des_index++] =*(arg_addr + arg_num++);
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
                    if (n == 0){
                        num[len] = 0;
                        len = 1;
                    }
                    for (int i = len-1; i >= 0; i--){
                        buf[des_index++] = num[i]+'0';
                    }
                    src_index++;
                    break;
                }
                case 's':{
                    char * str = *(char **)(arg_addr + arg_num++);
                    u32 str_len = strlen(str);
                    u32 i = 0;
                    while (i < str_len){
                        buf[des_index++] = str[i++];
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
                        buf[des_index++] = 0x0a;
                        src_index++;
                        break;
                    case 'r':
                        buf[des_index++] = 0x0d;
                        src_index++;
                        break;
                    default:
                    src_index++;
                        break;
                }
            }
        }
        else {
            buf[des_index++] = format[src_index++];
        }
    }
    buf[des_index] = 0;
    for (int i = 0; i < des_index; i++){
        putc(buf[i]);
    }
    return ;
}

void printf(char * format, ...){
    vprintf(format, &format+1);// 其实这里由于format是个指针，是32位的，所以这里对format的地址+1，也会加4个字节。
    return ;
}


// void _install_interrupt_handler(u8 n, u16 segment_address, u16 entry_offset){
//     write_memory_word(n*4, entry_offset);
//     write_memory_word(n*4+2, segment_address);
//     return ;
// }


// void install_interrupt_handler(u8 n, void (*interrupt_handler)()){
//     // 猜想：TODO: 函数指针是代码段的偏移量，也就是eip
//     // 猜想成立。
//     _install_interrupt_handler(n, 0x1000, interrupt_handler);
//     return ;
// }



int isalpha (int ch){
    if ('a' <= ch && ch <= 'z')
        return 1;
    if ('A' <= ch && ch <= 'Z')
        return 1;
    return 0;
}
int isdigit (int ch){
    if ('0' <= ch && ch <= '9')
        return 1;
    return 0;
}

int isalnum (int ch){
    if ( isalpha(ch) || isdigit(ch) ){
        return 1;
    }
    return 0;
}
int ispunct (int ch){
    char punct[34] =  "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    for (int i = 0; i < 33; i++){
        if (ch == punct[i])
            return 1;
    }
    return 0;
}
int isblank (int ch){
    if (ch == ' ')
        return 1;
    return 0;
}
int isprint (int ch){
    if (isalpha(ch) || isdigit(ch) || isalnum(ch) || ispunct(ch) || isblank(ch)){
        return 1;
    }
    return 0;
}
int iscntrl (int ch){
    //he control characters are the characters with the codes 0x00-0x1F and 0x7F.
    if (0 <= ch && ch <= 0x1f)
        return 1;
    if (ch == 0x7f)
        return 1;
    return 0;
}

/* tested
读取指定逻辑扇区号的扇区到指定内存地址处
注意一次最多写一个段，也就是64k 最多读128个扇区。
够用了够用了 */
void read_n_sector(u16 sector_code, u16 number,  u16 segment, u16 offset){
    for (int i = 0; i < number; i++){
        int sector = (sector_code+i) % 18+1;
        int mid = (sector_code+i) / 18;
        int cylinder = mid >> 1;
        int head = mid & 1;
        read_sector(head, cylinder,sector,segment, offset + i*512);
    }
    return ;
}


// /* 
// 读取内存中指定地址起n字节的内容到指定指针处 */
// void read_memory(u16 segment, u16 offset, u16 byte_size, u16 * ptr){
//     int addr = (segment << 4) + offset;
//     for (int i = 0; i < byte_size; i++){
//         *(ptr+i) = read_memory_word(addr);
//     }
// }