extern choose

[section .data]

num1st dd 3
num2nd dd 4

[section .text]

global _start
global myprint

_start:
    push dword [num2nd]
    push dword [num1st]
    call choose
    add esp, 0

    mov ebx, 0
    mov eax, 1
    int 0x80

myprint:
    mov edx, [esp+8]
    mov ecx, [esp+4]
    mov ebx, 1
    mov eax, 4
    int 0x80
    ret