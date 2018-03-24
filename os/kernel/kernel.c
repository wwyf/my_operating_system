// #include "../include/string.h"
#include "../include/type.h"
#include "../include/basic.h"

int main(){
    // init_kernel();
    // write_memory_word(0xb8000+2, 0x0730);
    int addr = 0xb8000;
    char c = '0';
    for(int i = 0; i < 600; i++){
        write_memory_byte(addr, c);
        write_memory_byte(addr+1, 0x07);
        addr = addr+2;
        c++;
    }
    while (1){}
    // install_int40();
    // show_welcome_screen();
}