#include "../include/type.h"
#include "../include/basic.h"
#include "../include/stdio.h"
#include "../include/string.h"


u16 system_call[MAX_SYSTEM_CALL];

void test_system_call();

int cstart(){
    install_system_call(2, test_system_call);
    // set_cursor(0);
    // for (int i = 0; i < 2000; i++)
    //     putc(' ');
    // set_cursor(0);
    // printf("%csaf%ssdfs%dasdf\n\r%csadf%ssf%d123\n\r", 'r',"test", 12344, 'c', "!!test!!", 999986);
    // install_interrupt_handler(0x40, my_int40);
    // __asm__(".byte 0x66");
    // __asm__(".byte 0x67");
    // __asm__("int $0x40");
    // while (1) {}
    return 0;
}

void test_system_call(){
    printf("123421431241234213412");
}