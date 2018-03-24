// #include "../include/string.h"
#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"

int main(){
    // init_kernel();
    // write_memory_word(0xb8000+2, 0x0730);
    // int addr = 0xb8000;

    set_cursor(0);
    for (int i = 0; i < 2000; i++){
        putc(' ');
    }
    set_cursor(0);
    for (char i = '0'; i <= 'z'; i++){
        putc(i);
    }

    // set_cursor(0);
    puti(2372342);

    // int a = get_cursor();
    // set_cursor(1000);
    while (1){}
    // install_int40();
    // show_welcome_screen();
}