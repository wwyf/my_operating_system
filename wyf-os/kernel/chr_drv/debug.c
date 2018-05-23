#include <basic.h>
#include <type.h>
#include <common/string.h>
#include <chr_drv/console.h>

#define INT_LENGTH 20
#define BUF_LENGTH 100

void _put_char(char c, uint16_t cursor_index){
    int _address = 0xb8000;
    int _offset = cursor_index * 2;
    char * display_ptr = (char *)(_address+_offset);
    *display_ptr = c;
    *(display_ptr+1) = 0x0007;
    return ;
}

void putc(char c){
    uint16_t cursor_index = _console_get_cursor();
    uint16_t row = cursor_index / 80;
    uint16_t col = cursor_index % 80;
    // if (cursor_index >= 1920){
    //     scroll_screen();
    //     cursor_index = 1840;
    // }
    switch (c) {
        case '\n':
            _console_set_cursor((row+1)*80); // 回车，移到下一行
            break;
        case '\r':
            _console_set_cursor(row*80);    // 移到本行开头处
            break;
        default:
            _put_char(c, cursor_index);
            _console_set_cursor(cursor_index+1);
            break;
    }
    return ;
}

void _putc(char c){
    uint16_t cursor_index = _console_get_cursor();
    uint16_t row = cursor_index / 80;
    uint16_t col = cursor_index % 80;
    switch (c) {
        case '\n':
            _console_set_cursor((row+1)*80+col); // 回车，移到下一行
            break;
        case '\r':
            _console_set_cursor(row*80);    // 移到本行开头处
            break;
        default:
            _put_char(c, cursor_index);
            // set_cursor(cursor_index+1);
            break;
    }
    return ;
}

void puti(uint32_t n){
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



void sprint(char * dest , char * format, ...){
    int arg_num = 0;
    int src_index = 0;   // 源字符串索引，总是指向未读的一位
    int des_index = 0;   // 目标字符串索引，总是指向未写的一位。
    int * arg_addr =(int *)&format+1; // 边长参数第一个参数的地址。
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
                    uint32_t str_len = com_strlen(str);
                    uint32_t i = 0;
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


void vprint(char * format, char ** va){
    char buf[BUF_LENGTH];
    int arg_num = 0;
    int src_index = 0;   // 源字符串索引，总是指向未读的一位
    int des_index = 0;   // 目标字符串索引，总是指向未写的一位。
    int* arg_addr = (int *)va; // 边长参数第一个参数的地址。
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
                    if (n == 0){
                        num[len] = 0;
                        len = 1;
                    }
                    else{
                        while (n != 0){
                            num[len] = n % 10;
                            n = n / 10;
                            len++;
                        }
                    }
                    for (int i = len-1; i >= 0; i--){
                        buf[des_index++] = num[i]+'0';
                    }
                    src_index++;
                    break;
                }
                case 's':{
                    char * str = *(char **)(arg_addr + arg_num++);
                    uint32_t str_len = com_strlen(str);
                    uint32_t i = 0;
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

void com_print(char * format, ...){
    vprint(format, &format+1);// 其实这里由于format是个指针，是32位的，所以这里对format的地址+1，也会加4个字节。
    return ;
}


// void _install_interrupt_handler(u8 n, uint16_t segment_address, uint16_t entry_offset){
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

