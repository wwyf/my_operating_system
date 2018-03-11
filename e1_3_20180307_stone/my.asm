; 程序源代码（stone.asm）
; 本程序在文本方式显示器上从左边射出一个*号,以45度向右下运动，撞到边框后反射,如此类推.
;  凌应标 2014/3
    Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
    Up_Rt equ 2                  ;
    Up_Lt equ 3                  ;
    Dn_Lt equ 4                  ;
    delay equ 50000					; 计时器延迟计数,用于控制画框的速度
    ddelay equ 5					; 计时器延迟计数,用于控制画框的速度
    color equ 10
    org 7c00h	; 给相对偏移加上7c00h这一个偏移量
start:
	mov ax,0B800h				; 文本窗口显存起始地址
	mov gs,ax					; GS = B800h
    call clean_screen
    mov si, 7
    mov di, 0

check_keyboard:
    call loop1
	call display_infomation
	mov ah, 01
	int 16h
	; 如果没有按， zf为0
	; 如果有按，往下执行
	jz check_keyboard

	; 从键盘读入字符,扫描码读进ah,ascII码读进al
	mov ah, 00h
	int 16h

	; 判断字符
	cmp al, 'c'
	mov word [status], 0720h
	call clean_screen
	; cmp al, 'h'
	; mov word [status], 0f65h
	; je clean_screen
	jmp check_keyboard
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; si:x
; di:y
loop1:
	dec word[count]			; 递减计数变量
	jnz loop1					; >0：跳转;
	mov word[count],delay
	dec word[dcount]			; 递减计数变量
      jnz loop1
	mov word[count],delay
	mov word[dcount],ddelay


    ; mov cx, 0f20h     ; 调用函数  清除当前字母的图像,准备显示下一个字母
    ; call show
; clean_current_char: ; 清除当前字母所占显存位置,准备画下一个字母显存
;       xor ax,ax                 ; 计算显存地址
;       mov ax,si
; 	mov bx,80
; 	mul bx
; 	add ax,di
; 	mov bx,2
; 	mul bx
; 	mov bx,ax
; 	mov ah,07h				
; 	mov al,20h		
; 	mov [gs:bx],ax  		

      mov al,1
      cmp al,byte[rdul]
	jz  DnRt
      mov al,2
      cmp al,byte[rdul]
	jz  UpRt
      mov al,3
      cmp al,byte[rdul]
	jz  UpLt
      mov al,4
      cmp al,byte[rdul]
	jz  DnLt
      jmp $	
;------------------------字母移动部分---------------------------------
DnRt:
	inc si 
	inc di
	mov bx,si
	mov ax,25
	sub ax,bx
      jz  dr2ur
	mov bx,di
	mov ax,80
	sub ax,bx
      jz  dr2dl
	jmp move_and_exit
dr2ur:
      mov si,23
      mov byte[rdul],Up_Rt	
      jmp move_and_exit
dr2dl:
      mov di,78
      mov byte[rdul],Dn_Lt	
      jmp move_and_exit

UpRt:
	dec si
	inc di
	mov bx,di
	mov ax,80
	sub ax,bx
      jz  ur2ul
	mov bx,si
	mov ax,-1
	sub ax,bx
      jz  ur2dr
	jmp move_and_exit
ur2ul:
      mov di,78
      mov byte[rdul],Up_Lt	
      jmp move_and_exit
ur2dr:
      mov si,1
      mov byte[rdul],Dn_Rt	
      jmp move_and_exit
UpLt:
	dec si
	dec di
	mov bx,si
	mov ax,-1
	sub ax,bx
      jz  ul2dl
	mov bx,di
	mov ax,-1
	sub ax,bx
      jz  ul2ur
	jmp move_and_exit


ul2dl:
      mov si,1
      mov byte[rdul],Dn_Lt	
      jmp move_and_exit
ul2ur:
      mov di,1
      mov byte[rdul],Up_Rt	
      jmp move_and_exit
DnLt:
	inc si
	dec di
	mov bx,di
	mov ax,-1
	sub ax,bx
      jz  dl2dr
	mov bx,si
	mov ax,25
	sub ax,bx
      jz  dl2ul
	jmp move_and_exit

dl2dr:
    mov di,1
    mov byte[rdul],Dn_Rt	
    jmp move_and_exit
	
dl2ul:
    mov si,23
    mov byte[rdul],Up_Lt	
    jmp move_and_exit

move_and_exit:
    mov cx, si
    shl cx, 8 
    mov cl, 02h 
    call show
    ret

;----------------------------字母移动部分----------------------------
; 显示字母函数
; 对外接口为ch, 控制显示格式
; 对外接口为cl, 控制显示字母
show:	                ; 计算显存地址
    mov ax, si
	mov bx,80
	mul bx
	add ax, di
	mov bx,2
	mul bx
	mov bx,ax
	mov ax,cx			;  AL = 显示字符值（默认值为20h=空格符）
	mov [gs:bx],ax  		;  显示字符的ASCII码值
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 清屏函数
; 被调用函数不保存任何寄存器,需要改进	
clean_screen:
	xor cx, cx
clean_screen_x_loop:
	mov dx, cx
	push cx
	xor cx, cx
clean_screen_y_loop:
	xor ax, ax
	pop dx      ; 为了能够在内循环里有一个与外循环相关联的量,并且不被内循环的add语句打乱,每次用到这个量的时候都从栈中拿出来
	push dx
	mov ax, dx  ; x
	mov bx, 80
	mul bx
	add ax, cx  ; y ; 会修改dx
	mov bx, 2
	mul bx
	mov bx, ax
	; mov ah, 07h ; 与默认相同
	; mov al, 20h ; 显示空格,即将该格显示的内容清除
	mov ax, word [status]
	mov [gs:bx], ax
	inc cx      
	cmp cx, 80 ; 若y等于80,则该行清空完成,转到下一行
	jz clean_screen_next_x
	jmp clean_screen_y_loop
clean_screen_next_x:
	pop cx
	inc cx
	cmp cx, 25 ; 若x等于25,则所有列清空完成
	jz clean_screen_exit
	jmp clean_screen_x_loop
clean_screen_exit:
    ret
;-------------展示名字-----------------------------
display_infomation:
	xor cx, cx                ; 计数器清零
      xor ax,ax                 ; 计算显存地址
      mov ax, 24
	mov bx,80
	mul bx
	add ax,57
	mov bx,2
	mul bx
	mov bx,ax
    push si
display_infomation_loop:
	mov ah,byte[dcount]				;  0000：黑底、1111：亮白字（默认值为07h）TODO:
	mov bp,name		;  AL = 显示字符值（默认值为20h=空格符）
	mov si, cx
	mov al, byte [bp+si]
	mov [gs:bx],ax 
	add bx, 2  		;  显示字符的ASCII码值
	inc cx
	cmp cx, 23  ; 个人信息长度
	jnz display_infomation_loop
    pop si
	ret

datadef:	
    count dw delay
    dcount dw ddelay
    rdul db Dn_Rt         ; 向右下运动
    ; x    db 7
    ; y    db 0
    ; char db 2
    name db '16337237 wang yong feng'
    status dw 0720h
;     number db '16337237'


times 510-($-$$) db 0
dw 0xaa55


