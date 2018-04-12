%include "../include/macro.inc"
org 0x4000
; section stone vstart=0x50000
    delay equ 50000					; 计时器延迟计数,用于控制画框的速度
    ddelay equ 5					; 计时器延迟计数,用于控制画框的速度
start:
	;xor ax,ax					; AX = 0   程序加载到0000：100h才能正确执行
    mov ax,cs
	mov ds,ax	
	mov ss,ax				; DS = CS
	mov es,ax					; ES = CS
	mov ax,0B800h				; 文本窗口显存起始地址
	mov gs,ax					; GS = B800h
	call clean_screen

	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 40 		 ; 列号=0
	mov dh, 23		       ; 行号=0
	mov cx, user2_MessageLength  ; CX = 串长（=9）
	mov bp, user2_Message		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
loop1:
	dec word[count]			; 递减计数变量
	jnz loop1					; >0：跳转;
	mov word[count],delay
	dec word[dcount]			; 递减计数变量
      jnz loop1
	mov word[count],delay
	mov word[dcount],ddelay

check_keyboard:
    mov ah, 01h
    int 16h
    ; 不断查询键盘缓冲区的状况
    ; 若有按键，则zf为0，若无按键，则zf为1，跳回去继续查询
    jz clean_current_char
    ; 有字符输入,从al中读取键盘输入
    mov ah, 00h
    int 16h

    cmp al, 'q' ; 如果键入q则退出
    jnz check_keyboard
	mov ax, 0x4c00
	int 40h
clean_current_char: ; 清除当前字母所占显存位置,准备画下一个字母显存
      xor ax,ax                 ; 计算显存地址
      mov ax,word[x]
	mov bx,80
	mul bx
	add ax,word[y]
	mov bx,2
	mul bx
	mov bx,ax
	mov ah,07h				
	mov al,20h		
	mov [gs:bx],ax  		;  显示字符的ASCII码值

check_x:
    mov ax, user2_bound_x_up
    cmp word [x], ax
    jz toggle_x_direct
    mov ax, user2_bound_x_down
    cmp word [x], ax
    jz toggle_x_direct
    jmp check_y
toggle_x_direct:
    mov ax, 0
    sub ax, word [x_direct]
    mov word [x_direct], ax
check_y:
    mov ax, user2_bound_y_left
    cmp word [y], ax
    jz toggle_y_direct
    mov ax, user2_bound_y_right
    cmp word [y], ax
    jz toggle_y_direct
    jmp char_move
toggle_y_direct:    
    mov ax, 0
    sub ax, word [y_direct]
    mov word [y_direct], ax

char_move:
    mov ax, word [x_direct]
    add word [x], ax
    mov ax, word [y_direct]
    add word [y], ax
show:	
    xor ax,ax                 ; 计算显存地址
    mov ax,word[x]
	mov bx,80
	mul bx
	add ax,word[y]
	mov bx,2
	mul bx
	mov bx,ax
	mov ah,bh				;  0000：黑底、1111：亮白字（默认值为07h）
	mov al,byte[char]			;  AL = 显示字符值（默认值为20h=空格符）
	mov [gs:bx],ax  		;  显示字符的ASCII码值
	jmp loop1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 清屏函数
; 被调用函数不保存任何寄存器,需要改进	
clean_screen:
	; 存放寄存器
	push ax
	push cx
	push si
	push es
	; 设置段寄存器
	mov ax, 0xB800
	mov es, ax
	mov cx, 2000
	mov si, 0
clean_screen_loop:
	mov byte [es:si], 20h
	inc si
	mov byte [es:si], 07h
	inc si
	loop clean_screen_loop
clean_screen_exit:

    mov ax, 0b800h
    mov es, ax
    mov ax, 0702h
    mov [es:0x00], ax

	pop es
	pop si
	pop cx
	pop ax
	ret

end:
    jmp $                   ; 停止画框，无限循环 

datadef:	
    count dw delay
    dcount dw ddelay
    x    dw user4_bound_x_up+1
    x_direct dw 1
    y    dw user4_bound_y_left+1
    y_direct dw 1
    char db 2

user2_Message:
    db 'Enter q to return system menu!                                                 '
user2_MessageLength equ ($-user2_Message)



