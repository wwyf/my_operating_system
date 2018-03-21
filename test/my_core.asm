; 内核程序
; 在mbr中已经写好了内核在软盘和在内存中的位置
; 软盘第2-5个扇区中
; 加载到内存 0x1000处
%include "macro.inc"
section my_core_header vstart=0

; 重定向段表
redirect:
    ; 将ds指向内核首部，为之后重定位内核段表
    mov ax, 0x0100
    mov ds, ax
    ; 重定位内核段表
    shr word [code_segment], 4
    add word [code_segment], 0x0100
    shr word [data_segment], 4
    add word [data_segment], 0x0100
    shr word [stack_segment], 4
    add word [stack_segment], 0x0100
    jmp far [code_entry]


; 内核首部，指明第一条指令偏移地址,及代码段段地址
; 经过上面的重定向程序后，能够将下面的汇编地址转变成实际运行的地址。
code_entry dw code_start
code_segment dw section.code.start
data_segment dw section.data.start
stack_segment dw section.stack.start
core_entry dw core_start

;###################################################################################33
;---------------------------------代码段--------------------------------------
section code align=16 vstart=0
;----------------------------------公用例程部分，相当于系统调用----------------------------
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


    mov ax, ds
    shl ax, 4
    add ax, feature_message
    mov bp, ax
    mov ax, 0
    mov ds, ax
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


; ----------------------------------------------------------
; [内核态]加载.com简单用户程序
; 固定加载到从物理地址10000h开始的内存中
; 传入参数：ax, 这里的扇区号从0开始算起，会有一个函数负责换算。
; 过程效果：从ax开始的两个扇区被加载到内存1000:0000对应位置
load_com_user_program:
    push ax
    push bx
    push cx
    push dx
    push es

    ; ax 已经准备好，不能修改
	mov bx,0x1000               ;段地址 ; 存放数据的内存基地址
	mov es,bx                ;设置段地址（不能直接mov es,段地址）
	mov bx,0 ;偏移地址; 存放数据的内存偏移地址
    mov cl, 18
    call ReadSector

    pop es
    pop dx
    pop cx
    pop bx
    pop ax
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
;###################################################################################
;--------------------------------内部过程-------------------------------------------
; 安装40号中断，用于用户程序返回内核
install_int40:
    push ax
    push bx
    push ds

    ; 安装 int 40 主要代码
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x40*4], int40_for_return 
    mov word [0x40*4+2], ax
    
    pop ds
    pop bx
    pop ax
    ret

; 40号中断的功能是将控制权从用户程序转到内核
; 务必要设计好0x40号中断，设计成与CPU状态无关，不依赖段寄存器的值
; 因为无论段寄存器是何值，都有可能会运行这条程序
; 执行中断的时候会将地址还有符号寄存器存到堆栈中
; 考虑先将堆栈中的东西pop出来，然后转移堆栈到内核栈，
; 修改各段指针后，再push内核的cs和指令偏移地址，通过iret回到内核
int40_for_return:

    ; 在用户栈中
    pop ax ; pop 原调用中断的偏移地址
    pop ax ; pop 原调用中断的段地址
    pop bx ; pop 用户的标志寄存器

    ; 修改段寄存器：ds, ss, es
    mov ax, core_header_data_segment
    mov ds, ax
    mov es, ax
    mov ax, word [core_stack_segment_header_offset]
    mov ss, ax
    mov sp, core_stack_length

    ; 保证栈为内核状态
    push bx ; push 标志寄存器 可能要修改
    push word [core_code_segment_header_offset]
    push word [core_entry_header_offset]

    ; 设置数据段寄存器
    mov ax, [core_data_segment_header_offset]
    mov ds, ax

    ; 可以运行任何在内核态的程序啦
    call clean_screen

    iret ; 成功回到内核

;--------------------------安装8号中断----------------------------
;
install_int8:
    push ax
    push ds
    push es

    mov al,34h   ; 设控制字值 
    out 43h,al   ; 写控制字到控制字寄存器 
    mov ax,0ffffh ; 中断时间设置
    out 40h,al   ; 写计数器 0 的低字节 
    mov al,ah    ; AL=AH 
    out 40h,al   ; 写计数器 0 的高字节
 
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [4*8], new_int8 ; 设置偏移地址为子过程所在位置
    mov word [4*8+2], ax ; 设置段地址为cs

    ; mov ax, 0b800h
    ; mov es, ax
    ; mov ax, 0730h
    ; mov [es:0x00], ax

    pop es
    pop ds
    pop ax
    ret
;------------------------------------------------------------------------------
new_int8:
    push es
    push ax
    push bx
    push cx
    push dx
    push bp

    ; 设置段地址
    mov ax, 0b800h
    mov es, ax
; for i = 24:1
    ; [es:160*i] = [es:160*(i-1)]
    
    ; 计算左下角所在地址，并取出左下角的字符
    mov cx, 24
    mov al, 160
    mul cl
    mov bp, ax
    mov dl, byte [es:bp] ; [es:160*24]
    ; 向下平移
    sub bp, 160            ;[es:160*23]
    ; 将当前行的字符移到下一行
scroll_bound_loop_1:
    mov al, byte [es:bp]
    mov byte [es:bp+160], al
    call get_random
    mov byte [es:bp+161], al
    sub bp, 160
    loop scroll_bound_loop_1

    ; 将第一行的1-79的字符移到0-78
    mov cx, 79
    mov bp, 2
scroll_bound_loop_2:
    mov al, byte [es:bp]
    mov byte [es:bp-2], al
    call get_random
    mov byte [es:bp-1], al
    inc bp
    inc bp
    loop scroll_bound_loop_2

    mov bp, 158+160
    mov cx, 24
scroll_bound_loop_3:
    mov al, byte [es:bp]
    mov byte [es:bp-160], al
    call get_random
    mov byte [es:bp-159], al
    add bp, 160
    loop scroll_bound_loop_3

    sub bp, 160
    sub bp, 2
    mov cx, 78
scroll_bound_loop_4:
    mov al, byte [es:bp]
    mov byte [es:bp+2], al
    call get_random
    mov byte [es:bp+3], al
    sub bp, 2
    loop scroll_bound_loop_4

    add bp, 2
    mov byte [es:bp], dl
    call get_random
    mov byte [es:bp+1], al
    pop bp
    pop dx
    pop cx
    pop bx
    pop ax
    pop es

	mov al,20h			; AL = EOI
	out 20h,al			; 发送EOI到主8529A
	out 0A0h,al			; 发送EOI到从8529A， 注释掉好像也行，为啥？

    iret

;##############################################################################
;------------------------------------内核初始化----------------------------------
code_start:
    ; 先进行清屏
	call clean_screen
    ; 初始化内核段地址
    ; 此时ds指向header
    ; cs 指向正确的位置。
    mov ax, ds
    mov es, ax
    ; es 指向header
    mov ax, [stack_segment]
    mov ss, ax
    mov sp, stack_end
    mov ax, [data_segment]
    mov ds, ax

    ; 安装中断
    ; 安装40号中断，用于返回
    call install_int40
    ; call install_int8
    ; 内核已加载完成，按enter继续
    call display_core_massage



check_key_board_load_core:
    mov ah, 01h
    int 16h
    ; 不断查询键盘缓冲区的状况
    ; 若有按键，则zf为0，若无按键，则zf为1，跳回去继续查询
    jz check_key_board_load_core
    ; 有字符输入
    mov ah, 00h
    int 16h

    cmp al, 0x0d
    jnz check_key_board_load_core
    
check_key_board_load_core_end:


;----------------------------内核功能入口---------------------------------------
core_start:
    call clean_screen
    call show_welcome_screen
; 这里放的是内核加载器，负责加载在其他扇区的程序。
check_key_board_load_feature:
    mov ah, 01h
    int 16h
    ; 不断查询键盘缓冲区的状况
    ; 若有按键，则zf为0，若无按键，则zf为1，跳回去继续查询
    jz check_key_board_load_feature
    ; 有字符输入,从al中读取键盘输入
    mov ah, 00h
    int 16h

check_key_board_load_feature_1:
    cmp al, '1' 
    jnz check_key_board_load_feature_2
    mov ax, 18 
    jmp run_com
check_key_board_load_feature_2:
    cmp al, '2' 
    jnz check_key_board_load_feature_3
    mov ax, 36
    jmp run_com
check_key_board_load_feature_3:
    cmp al, '3' 
    jnz check_key_board_load_feature_4
    mov ax, 54
    jmp run_com
check_key_board_load_feature_4:
    cmp al, '4' 
    jnz check_key_board_load_feature_l
    mov ax, 72
    jmp run_com
check_key_board_load_feature_l:
    cmp al, 'l' 
    jnz check_key_board_load_feature_t
    mov ax, 90
    jmp run_com
check_key_board_load_feature_t:
    cmp al, 't' 
    jnz check_key_board_load_feature
    mov ax, 108
    jmp run_com
    jmp check_key_board_load_feature

; 根据al里面的值加载对应的用户程序
run_com:
    call load_com_user_program
    jmp run_com_user_program

;###############################################################################
;------------------------------常用过程-----------------------------------------
; 运行用户com程序前运行
; 效果：
; 将cs,ds,es,ss置为0x1000
; 将sp置为0x0400(相当于.com程序末尾)
run_com_user_program:
    call clean_screen
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ax, 0x0400
    mov sp, ax
    jmp 0x1000:0x0000
    
;---------------------------------------------------------------------------------


;###############################################################################
;-----------------------------------内核数据段-----------------------------------
section data align=16 vstart=0



Core_Message:
    db '                            Loading core completed!'
    db 0x0d ; 回车
    db 0x0a ; 换行
    db '                            Enter to continue....'
Core_MessageLength equ ($-Core_Message)

feature_message db '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'
db '*                                                                              *'
db '*                                                                              *'
db '*                                                                              *'
db '*                                                                              *'
db '*         ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||         *'
db '*         ||                  wyf OS is ready!                      ||         *'
db '*         ||                                                        ||         *'
db '*         ||       [1]:user program one(top left conrner)           ||         *'
db '*         ||       [2]:user program one(top right conrner)          ||         *'
db '*         ||       [3]:user program one(lower left conrner)         ||         *'
db '*         ||       [4]:user program one(lower right conrner)        ||         *'
db '*         ||       [l]:the experiment one                           ||         *'
db '*         ||       [t]:waiting for develop                          ||         *'
db '*         ||                                                        ||         *'
db '*         ||                                                        ||         *'
db '*         ||                                                        ||         *'
db '*         ||                                                        ||         *'
db '*         ||                                  copyright by wyf      ||         *'
db '*         ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||         *'
db '*                                                                              *'
db '*                  student number: 16337237                                    *'
db '*                           name : wang yong feng                              *'
db '*                                                                              *'
db '%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%'
feature_message_length equ ($-feature_message)



;###############################################################################
;----------------------内核栈---------------------------------------------------
section stack align=16 vstart=0
    times 256 db 0
stack_end:
