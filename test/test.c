/* test.c */
#include<stdio.h>
int main() {
    int a=10, b=15;
    // the asm code is : a = b
    asm ("mov eax, %1;\n"
         "mov %0, eax;"
          :"=r"(a)
          :"r"(b)
          :"eax"
        );
    printf("a=%d,b=%d\n",a,b);
    return 0;
}
// >  gcc -masm=intel test.c -o test