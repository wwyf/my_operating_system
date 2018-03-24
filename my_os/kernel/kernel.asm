; 已经在保护模式

start:
    mov eax, 0018h
    mov ds, ax
    mov byte [ds:0x00], '@'
    mov byte [ds:0x01], 07h
    jmp $