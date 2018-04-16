#include "../include/utilities.h"
#include "../include/stdio.h"

int test(int a, int b){
    printf("in function %d %d", a, b);
}

int main(){
    while(1){
        for(int i = 0; i < 10000000; i++);
        printf("%c ",'b');
        test(2,2);
    }
}



