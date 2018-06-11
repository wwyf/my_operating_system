#include <test/test.h>
#include <chr_drv/tty_drv.h>
#include <intr/interrupt.h>
#include <proc/process.h>
#include <hd_drv/hd.h>
#include <sys_tasks.h>

int main(){
    tty_init();
    interrupt_init();
    process_init();
    hd_init();
    task_init();
    
    _basic_cli();
    main_test();
    return 0;
}