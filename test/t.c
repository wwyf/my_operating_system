void display_char(char );
void return_point();
void display_string(char *, long);
void display_message(long);

char message[7]="test!!";

// #include<string.h>

int main(){
    int a = add(3,4);
    // int a = 6;
    display_char('@');
    for(int i = 0;i <= 1000000;i++);
    display_string(message,a);// 调用函数的时候，会将cs和ip（32位）的push进栈中，
    // display_message(a);
    return_point();
    return 0;
}

int add(int a, int b){
    return a+b;
}