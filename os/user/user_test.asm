org 0x2000
code_start:
	; 初始化用户程序段地址
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x200

	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0 		 ; 列号=0
	mov dh, 2		       ; 行号=0
	mov cx, user_messageLength  ; CX = 串长（=9）
	mov bp, user_message		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
    mov ax, 0b800h
    mov es, ax
    mov ax, 0702h
    mov [es:0x00], ax

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
    mov ax, 0x4c00
	int 40h

user_message:
    db '   You are in my test program now!'
    db 0x0d
    db 0x0a
    db '   Enter q to return system menu!'
user_messageLength equ ($-user_message)
