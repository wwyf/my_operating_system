; 已经在保护模式
%include "../include/gdt.inc"
[BITS 32]
start:
    mov ax, SelectorVideo
    mov ds, ax
    mov ax, 0732h
    mov word [ds:0x00], ax
    jmp $