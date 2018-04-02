#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"

#define MAX_BUF 100

void create_a_line();
void parser();
// u8 display_start; // 显存区域开头
// u8 display_end; // 显存区域末尾
int command_line_row = -1; // 命令行当前所在行号
int new_line_flag = 1;
int message_length = 0;
int command_line_cursor = 0; // 光标在输入区的偏移量
char input_buf[MAX_BUF]; // 输入缓存区


void tty(){
    int keyboard_flag = 0;
    new_line_flag = 1;
    command_line_row = -1;
    clean_screen();
    set_cursor(1974);
    printf(" wang yong feng 16337237 ");
    set_cursor(0);
    while (1){
        // 使用缓冲区数据更新
        if (new_line_flag){
            create_a_line();
            new_line_flag = 0;
        }

        if(command_line_row == 24 && message_length + command_line_cursor > 80){
            scroll_screen();
            command_line_row--;
            set_cursor(1920);
        }
        putc(' ');// 用来删除光标处的文字，与输入退格的时候光标-1配合使用

        // command_line_row = get_cursor() / 80;
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
                input_buf[command_line_cursor] = '\0';
                parser();
            }
            else if (ascii_code == 8){
                if(command_line_cursor){
                    input_buf[--command_line_cursor] = 0;
                    // 将光标设置为前一个位置，方便删除
                    int cur_cursor = get_cursor();
                    set_cursor(cur_cursor-1);
                }
            }
            else if (ascii_code == 27){
                __asm__("mov $0x4c00, %ax");
                __asm__("int $0x40");
                // printf("test!!esc!!");
            }
            // DEBUG 用于测试按键输入
            // int a = get_cursor();
            // set_cursor(1900);
            // printf("%d %d %d", ascii_code, scan_code, a);
            // set_cursor(a);
        }
        keyboard_flag = 0;
    }
    return ;
}


void parser(){
    char arguments[10][10];
    int input_start = 0;
    int input_end = 0;
    int arguments_num = 0;
    // 使用空格作为分隔符，将输入缓冲区的内容分割成几个参数分别存进arguments数组中
    while (input_end <= command_line_cursor){
        if(input_buf[input_end] == ' ' || input_buf[input_end] == '\0'){
            if (input_start == input_end){ // 如果并没有参数，那么继续
                input_start++;
                input_end++;
                continue;
            }
            else {// 如果有参数，并且已经读到末尾了，那就复制到相应的区域
                strncpy(arguments[arguments_num], input_buf+input_start, input_end-input_start);
                input_end++;
                input_start = input_end;
                arguments_num++;
            }
        }
        else {// 有参数，还没有读到末尾
            input_end++;
            continue;
        }
    }

    // 完成分词，开始进行解析。

    if(!strcmp(arguments[0],"run")){// 如果返回0，就是相等了
        // 运行一个用户程序。
        // 先从软盘中加载一个指定程序到内存处，然后开始执行。
        // 考虑维护一个空闲空间的指针，然后每加载一个程序，如果可行的话。
        void (**my_program)();
        *my_program = 0x2000;
        fs_load_by_name(arguments[1],*my_program);
        (*my_program)();
    }
    else if(!strcmp(arguments[0],"ls") && arguments_num == 1){
        printf("\n\n");
        fs_show_root_file_table();
    }
    else {
        printf("\n%s : command not found! \n", input_buf);
    }
    int current_cursor = get_cursor();
    command_line_row = (current_cursor / 80);
    // printf("arguments: %d|%s|%s|",arguments_num, arguments[0],arguments[1]);
    return ;
}

// 转移光标到命令行下一行,更新缓冲区，并且打印信息
void create_a_line(){
    char message[30] = "walker@walker-pc ~ $ \0";
    for (int i = 0; i < MAX_BUF; i++){
        input_buf[i] = 0;
    }
    message_length = strlen(message);
    command_line_row = command_line_row + (command_line_cursor + message_length)/80 + 1;
    while(command_line_row > 24){
        scroll_screen();
        command_line_row--;
    }
    command_line_cursor = 0;
    set_cursor(command_line_row * 80);
    // printf("%d", command_line_row);
    printf("%s", message);
}


void tty_scroll_screen(){
    if (get_cursor()/80 > 24){
        scroll_screen();
        command_line_row--;
    }
    return ;
}