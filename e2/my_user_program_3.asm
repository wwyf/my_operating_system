section my_user1_program_header vstart=0x10000

code_start:
	; 初始化用户程序段地址
    ; 此时ds指向header
    ; cs 指向正确的位置。
    ; mov ax, ds
    ; mov es, ax
    ; ; es 指向header
    ; mov ax, [stack_segment]
    ; mov ss, ax
    ; mov sp, stack_end
    ; mov ax, [data_segment]
    ; mov ds, ax
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0 		 ; 列号=0
	mov dh, 2		       ; 行号=0
	mov cx, user1_MessageLength  ; CX = 串长（=9）
	mov bp, user1_Message		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符

check_keyboard:
    mov ah, 01h
    int 16h
    ; 不断查询键盘缓冲区的状况
    ; 若有按键，则zf为0，若无按键，则zf为1，跳回去继续查询
    jz check_keyboard
    ; 有字符输入,从al中读取键盘输入
    mov ah, 00h
    int 16h

    cmp al, 'q' ; 如果键入q则退出
    jnz check_keyboard
	int 40h

user1_Message:
    db 'You are in my user3 program now!'
user1_MessageLength equ ($-user1_Message)
