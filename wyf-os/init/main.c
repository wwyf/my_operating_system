#include <test.h>
#include <tty_drv.h>
#include <interrupt.h>

int main(){
    tty_init();
    interrupt_init();
    process_init();
    test1();
    test2();
    return 0;
}