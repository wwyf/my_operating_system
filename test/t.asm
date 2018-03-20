[section .data]

strHello db "Hello, world!", 0Ah
STRLEN equ $ - strHello

[section .text]

global _start

_start:
    mov edx, STRLEN
    mov ecx, strHello
    mov ebx, 1
    mov eax, 4
    int 0x80
    mov ebx, 0
    mov eax, 1
    int 0x80