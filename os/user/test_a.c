#include "../include/utilities.h"
#include "../include/stdio.h"


int test(int a, int b){
    printf("in process %d ! %d\n", a, b);
}

int main(){
    while(1){
        for(int i = 0; i < 20000000; i++);
        printf("%c ",'a');
        test(1,1);
    }
}