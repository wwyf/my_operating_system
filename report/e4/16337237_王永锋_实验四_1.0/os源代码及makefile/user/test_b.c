#include "../include/utilities.h"
#include "../include/stdio.h"

int test(int a){
    printf("in process %d !\n", a);
}

int main(){
    printf("test in b");
    while(1){
        // __asm__("int $0x41");
        for (int i ='a'; i <= 'z'; i++){
            // 延时
            // for(int k = 0; k < 50000000; k++);
            printf("out put %c ",i);
            test(2);
        }
    }
}