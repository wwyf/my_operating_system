%ifdef DEBUG
    org 0100h
%else
    org 07c00h
%endif

mov ax, cs
mov ds, ax
mov es, ax
call DispStr
jmp $

DispStr:
mov ax, BootMessage
mov bp, ax
mov cx, 22
mov ax, 01301h
mov bx, 000ch
mov dl, 0
int 10h
ret

BootMessage: db "Hello, OS world of wyf!"
times 510-($-$$) db 0
dw 0xaa55