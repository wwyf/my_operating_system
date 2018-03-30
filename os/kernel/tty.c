#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/interrupt.h"

#define MAX_BUF 100

u8 display_start; // 显存区域开头
u8 display_end; // 显存区域末尾
int command_line_row = -1; // 命令行当前所在行号
u8 new_line_flag = 1;
u8 message_length = 0;
u16 command_line_cursor = 0; // 光标在输入区的偏移量
char input_buf[MAX_BUF]; // 输入缓存区


int main(){
    int keyboard_flag = 0;
    clean_screen();
    while (1){
        // 使用缓冲区数据更新
        if (new_line_flag){
            create_a_line();
            new_line_flag = 0;
        }
        set_cursor(80*command_line_row + message_length);
        puts(input_buf, command_line_cursor);

        // 更新完了 检测键盘
        keyboard_flag = check_keyboard();

        // 如果返回1，则有输入，处理输入信息，否则继续循环。
        if (keyboard_flag){
            // 处理键盘事件
            u16 keyboard_code =  get_keyboard();
            u8 ascii_code = keyboard_code;
            u8 scan_code = keyboard_code >> 8;
            if (isprint(ascii_code)){
                if (command_line_cursor <= MAX_BUF){
                    input_buf[command_line_cursor++] = ascii_code;
                }
            }
            else if (ascii_code == 13){
                new_line_flag = 1;
            }
            else if (ascii_code == 8){
                if(command_line_cursor){
                    input_buf[--command_line_cursor] = 0;
                }
            }
            int a = get_cursor();
            set_cursor(1900);
            printf("%d %d %d", ascii_code, scan_code, a);
            set_cursor(a);
        }
        keyboard_flag = 0;
    }
    return 0;
}


// 转移光标到命令行下一行,更新缓冲区，并且打印信息
void create_a_line(){
    char message[30] = "walker@walker-pc ~ $ \0";
    for (int i = 0; i < MAX_BUF; i++){
        input_buf[i] = 0;
    }
    message_length = strlen(message);
    command_line_row = command_line_row + (command_line_cursor + message_length)/80 + 1;
    command_line_cursor = 0;
    set_cursor(command_line_row * 80);
    printf("%s", message);
}

