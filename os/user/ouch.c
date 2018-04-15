#include "../include/utilities.h"
#include "../include/basic.h"
#include "../include/stdio.h"

int main(){
    printf("in the ouch!");
    int is_hit = check_keyboard();
    while(1){
        // 不断检查， 直到有端盘输入才停止
        while (!is_hit){
            is_hit = check_keyboard();
        }
        char input = get_keyboard();
        int cursor = get_random() % 2000;
        set_cursor(cursor);
        printf("ouch!");
        if (input == 'q'){
            break;
        }
    }
}
