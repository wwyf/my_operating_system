// #include "../include/type.h"
// #include "../include/basic.h"
// #include "../include/stdio.h"
// #include "../include/string.h"
// #include "../include/interrupt.h"

int main(){
    // set_cursor(10);
    // write_port_byte(0x3d4, 0x0e);
    // install_interrupt_handler(0x40, my_int40);
    // trigger_interrupt_int40();
    // __asm__("int $0x40");
    
    // set_cursor(0);
    // for (int i = 0; i < 2000; i++)
    //     putc(' ');

    // char test[10];
    // set_cursor(80);
    // sprintf(test, "%d", 123456789);
    // puts(test, 10);

    // set_cursor(160);
    // sprintf(test, "%c%c%c %c  %c %c\n", 'a','b','c','d','e','f');
    // puts(test, 10);

    // set_cursor(240);
    // sprintf(test, "%d", strlen("123456789123456789"));
    // puts(test,strlen(test));

    // set_cursor(320);
    // sprintf(test, "%s\n", "test!!ASFD");
    // puts(test, 11);

    // // set_cursor(400);
    // printf("%csaf%ssdfs%dasdf\n\r%csadf%ssf%d123\n\r", 'r',"test", 12344, 'c', "!!test!!", 999986);


    while (1){}
    return 0;
}


// void my_int40(){
//     set_cursor(1920);
//     printf("test success!");
//     return_from_interrupt();
// }