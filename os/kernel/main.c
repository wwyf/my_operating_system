#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/interrupt.h"

void my_int40();

int cstart(){


    // set_cursor(0);
    // for (int i = 0; i < 2000; i++)
    //     putc(' ');
    // set_cursor(0);
    // printf("%csaf%ssdfs%dasdf\n\r%csadf%ssf%d123\n\r", 'r',"test", 12344, 'c', "!!test!!", 999986);
    // install_interrupt_handler(0x40, my_int40);
    // __asm__(".byte 0x66");
    // __asm__(".byte 0x67");
    // __asm__("int $0x40");

    

    while (1) {}

    return 0;
}
