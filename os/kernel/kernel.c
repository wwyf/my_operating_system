// #include "../include/string.h"
#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"

int main(){
    // init_kernel();
    // write_memory_word(0xb8000+2, 0x0730);
    // int addr = 0xb8000;
    set_cursor(0);
    for (int i = 0; i < 2000; i++)
        putc(' ');
    char test[10];
    set_cursor(80);
    sprintf(test, "%d", 123456789);
    puts(test, 10);
    set_cursor(160);
    sprintf(test, "%c%c%c %c  %c %c\n", 'a','b','c','d','e','f');
    puts(test, 10);

    set_cursor(240);
    sprintf(test, "%d", strlen("123456789123456789"));
    puts(test,strlen(test));

    set_cursor(320);
    sprintf(test, "%s\n", "test!!ASFD");
    puts(test, 11);

    while (1){}
    return 0;
}