void display_char(char );
void return_point();
void display_string(char *, long);
void display_message(long);

char message[7]="test!!";

// #include<string.h>

int main(){
    long a = 6;
    display_char('@');
    for(int i = 0;i <= 1000000;i++);
    display_string(message,a);
    // display_message(a);
    return_point();
    return 0;
}