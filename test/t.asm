global display_char
global return_point
global display_string
global display_message

extern message
; extern user_messageLength
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
    ret
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
display_string:
    mov ax, cs
    mov es, ax
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 1 		 ; 列号=0
	mov dh, 2		       ; 行号=0
	mov ecx, [esp+8] ; CX = 串长（=9）
	mov ebp, [esp+4]		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
    db 0x66
    ret
display_message:
    mov ax, cs
    mov es, ax
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 1 		 ; 列号=0
	mov dh, 2		       ; 行号=0
	mov ecx, [esp+4] ; CX = 串长（=9）
	mov ebp, message		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
    db 0x66
    ret