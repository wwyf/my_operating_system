#include "../include/utilities.h"
#include "../include/basic.h"
#include "../include/stdio.h"

int main(){
    int is_hit = 0;
    printf("in the ouch!");
    while(1){
        // for(int i = 0; i < 50000000; i++);
        // 不断检查， 直到有端盘输入才停止
        is_hit = check_keyboard();
        while (!is_hit){
            is_hit = check_keyboard();
            // __asm__("int $0x41");
        }
        char input = get_keyboard();
        int cursor = get_random() % 1900;
        set_cursor(cursor);
        printf("ouch! Your input is %c !\n", input);
    }
}
