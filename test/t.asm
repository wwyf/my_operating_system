global display_char
global return_point
%macro retl 0
    db 0x66
    ret
%endmacro

bits 16
section .data

section .text

display_char:
    mov ax, 0B800h
    mov es, ax
    mov ecx, [esp+4] ; 1 argument
    mov [es:0x00], cx
    db 0x66
    retl
return_point:
    mov ah, 01h
    int 16h
    ; 不断查询键盘缓冲区的状况
    ; 若有按键，则zf为0，若无按键，则zf为1，跳回去继续查询
    jz return_point
    ; 有字符输入,从al中读取键盘输入
    mov ah, 00h
    int 16h

    cmp al, 'q' ; 如果键入q则退出
    jnz return_point
	int 40h
