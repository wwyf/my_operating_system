#include <test/test.h>
#include <chr_drv/tty_drv.h>
#include <intr/interrupt.h>
#include <proc/process.h>
#include <hd_drv/hd.h>

int main(){
    tty_init();
    interrupt_init();
    process_init();
    hd_init();
    
    main_test();
    return 0;
}