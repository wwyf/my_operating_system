#include "../include/utilities.h"
#include "../include/stdio.h"

int test(int a, int b){
    printf("in process %d ! %d\n", a, b);
}

int main(){
    printf("test in b");
    while(1){
        for(int i = 0; i < 20000000; i++);
        // __asm__("int $0x41");
        printf("%c ",'b');
        test(2,2);
    }
}



