#include "../include/utilities.h"
#include "../include/basic.h"
#include "../include/stdio.h"

int main(){
    int is_hit = 0;
    while(1){
        for(int i = 0; i < 20000000; i++);
        printf("in the ouch!");
        // 不断检查， 直到有端盘输入才停止
        while (!is_hit){
            is_hit = check_keyboard();
            // printf("%d", is_hit);
        }
        char input = get_keyboard();
        int cursor = get_random() % 1900;
        set_cursor(cursor);
        printf("ouch! %c ", input);
        is_hit = 0;
    }
}
