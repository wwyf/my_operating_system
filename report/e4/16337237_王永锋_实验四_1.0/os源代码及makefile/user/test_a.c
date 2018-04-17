#include "../include/utilities.h"
#include "../include/stdio.h"


int test(int a){
    printf("in process %d !\n", a);
}

int main(){
    printf("test in a");
    while(1){
        // __asm__("int $0x41");
        for (int i = 0; i < 1000; i++){
            // 延时
            // for(int k = 0; k < 50000000; k++);
            printf("out put %d ",i);
            test(1);
        }
    }
}