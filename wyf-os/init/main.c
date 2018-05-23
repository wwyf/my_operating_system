#include <test/test.h>
#include <chr_drv/tty_drv.h>
#include <intr/interrupt.h>

int main(){
    tty_init();
    interrupt_init();
    process_init();
    
    main_test();
    return 0;
}