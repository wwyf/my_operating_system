#include <interrupt.h>
#include <head.h>
#include <protect.h>
#include <debug.h>
#include <kernel.h>



void interrupt_handler(uint8_t n){
    print("in the %d interrupt!!!", n);
}

void interrupt_init(){
    for (int i = 0; i < 256; i++){
        // print("%d  ", interrupt_table[i]);
        // printk("%x  ", ((uint32_t)&interrupt_table + 8 * i));
        set_intr_gate(i, (void *)((uint32_t)&interrupt_table + 8 * i));
    }
}