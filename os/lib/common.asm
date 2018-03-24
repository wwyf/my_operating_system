;----------------------------------------------------------------------------------------
; 清屏函数
; 无需关心寄存器，该清屏函数能够保存内部用过的所有寄存器，并在结束时恢复原有的值。
; 在内核栈中
; 无依赖
clean_screen:
	; 存放寄存器
	push ax
    push bx
	push cx
    push dx
	push si
	push es
    push bp
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
    pop bp
	pop es
	pop si
    pop dx
	pop cx
    pop bx
	pop ax
	ret


;------------------------------------------------------------------------------------
; [内核态]显示内核信息
; 似乎内核信息不能正确显示？？
; 因为在调用该程序前，ds已经经过重定位，不能够直接访问到首部数据，必须通过es段寄存器存放的首部地址来访问。
; [依赖]  ds 的正确取值
display_core_massage:
    push es
    push ax
    push bx
    push cx
    push dx
    push bp

    mov ax, ds ; 注意，要访问首部数据的话，需要通过es来访问，ds已经被修改了
    mov es, ax
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0 		 ; 列号=0
	mov dh, 12		       ; 行号=0
	mov cx, Core_MessageLength  ; CX = 串长（=9）
	mov bp, Core_Message		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符

    pop bp
    pop dx
    pop cx
    pop bx
    pop ax
    pop es
    ret


;------------------------------------------------------------------------------------
; [内核态]显示欢迎屏幕函数
; 屏幕上写好了可能有的各种功能，此函数仅仅是打印页面，键盘的控制在内核中实现。
; [依赖] 内核态中ds的正确取指
show_welcome_screen:
    ; TODO: 修改，显示颜色背景，显示功能列表
    push es
    push ax
    push bx
    push cx
    push dx
    push bp
    push si
    push di
    push ds

; 设置背景色
	; 设置段寄存器
	mov ax, 0xB800
	mov es, ax

	mov cx, 2000
	mov si, 0
fill_screen_loop:
	mov byte [es:si], 20h
	inc si
	mov byte [es:si], 70h
	inc si
	loop fill_screen_loop


    mov ax, feature_message
    mov bp, ax
	mov cx, 2000
	mov si, 0
display_welcome_screen_loop:
    mov di, 2000
    sub di, cx
    mov al, [ds:bp+di]  ; 这条语句出了问题 为什么[ds:si]就读不了呢？
	mov byte [es:si], al
	inc si
	mov byte [es:si], 71h
	inc si
	loop display_welcome_screen_loop
    
    pop ds
    pop di
    pop si
    pop bp
    pop dx
    pop cx
    pop bx
    pop ax
    pop es
    ret



;------------------------------------------------------------
;----------------------------------------------------------------------------
; 函数名: ReadSector
; http://blog.csdn.net/littlehedgehog/article/details/2147361
;----------------------------------------------------------------------------
; 作用:
;    从第 ax 个 Sector 开始, 将 cl 个 Sector 读入 es:bx 中
;    从0开始算
ReadSector:
    ; -----------------------------------------------------------------------
    ; 怎样由扇区号求扇区在磁盘中的位置 (扇区号 -> 柱面号, 起始扇区, 磁头号)
    ; -----------------------------------------------------------------------
    ; 设扇区号为 x
    ;                           ┌ 柱面号 = y >> 1
    ;       x           ┌ 商 y ┤
    ; -------------- => ┤      └ 磁头号 = y & 1
    ;  每磁道扇区数     │
    ;                   └ 余 z => 起始扇区号 = z + 1
    push   bp
    mov    bp, sp
    sub    sp, 2            ; 辟出两个字节的堆栈区域保存要读的扇区数: byte [bp-2]

    mov    byte [bp-2], cl
    push   bx            ; 保存 bx
    mov    bl, 18    ; bl: 除数  [每磁道扇区数]
    div    bl            ; y 在 al 中, z 在 ah 中
    inc    ah            ; z ++
    mov    cl, ah            ; cl <- 起始扇区号
    mov    dh, al            ; dh <- y
    shr    al, 1            ; y >> 1 (其实是 y/BPB_NumHeads, 这里BPB_NumHeads=2)
    mov    ch, al            ; ch <- 柱面号
    and    dh, 1            ; dh & 1 = 磁头号
    pop    bx            ; 恢复 bx
    ; 至此, "柱面号, 起始扇区, 磁头号" 全部得到 ^^^^^^^^^^^^^^^^^^^^^^^^
    mov    dl, 0        ; [驱动器号 (0 表示 A 盘)]
.GoOnReading:
    mov    ah, 2            ; 读
    mov    al, byte [bp-2]        ; 读 al 个扇区
    int    13h
    jc    .GoOnReading        ; 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止

    add    sp, 2
    pop    bp

    ret

;--------------------------------------------------------------
; 取得随机数，结果保存在ax中
; 会修改ax！！作为返回值！！
get_random:
    push bx

    mov ax, 00h
    out 43h, al
    in al, 40h

    mov bl, 8 ; 除以8，得到范围为0-7的随机数
    div bl

    mov al, ah
    mov ah, 0h
    add ax, 114 ; 加上白色背景色
    pop bx
    ret