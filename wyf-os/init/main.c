int main(){
    int a = 2;
    __asm__("int $0x80");
    return 0;
}