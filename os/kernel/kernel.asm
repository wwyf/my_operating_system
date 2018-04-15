; 内核程序
; 加载到内存 0x10000处
%include "../include/macro.inc"
extern system_call
extern cstart
extern tty
extern get_random
extern move_name
extern my_infomation
[bits 16]
;----------------------------内核功能入口---------------------------------------
_start:
    call install_int8
    call install_int33
    call install_int34
    call install_int35
    call install_int36
    call install_int40
    call dword cstart
start_tty:
    mov ah, 0x02
    int 0x40
    call dword tty
    jmp $
    jmp $
; 这里放的是内核加载器，负责加载在其他扇区的程序。


;##############################################################################
;--------------------------------安装40号中断----------------------------------
; 安装40号中断，用于用户程序返回内核
install_int40:
    proc_save

    ; 安装 int 40 主要代码
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x40*4], new_int40
    mov word [0x40*4+2], ax
    
    proc_recover
    ret

;------------------------------------------------------------------------------
; 这是新的int40，用于调用系统调用
; 使用ax索引中断
; 每一个项是16位+16位 
new_int40:

    cmp ah, 0x4c
    je .return_kernel
    ;TODO:
    ; 保存用户栈指针
    ; 将栈切换到内核栈
    ; 将用户栈保存到内核栈中

    mov bl, ah
    xor ax, ax
    mov al, 0x2
    mul bl
    mov si, ax
    mov bx, system_call
    call dword [bx + si] ; 注意这个call是32位的。
    ;TODO:
    ; 从内核栈恢复到用户栈
    iret

 .return_kernel:
    pop cx
    pop cx
    pop cx

    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x5000

    push cx
    push 0x1000
    push start_tty
    sti
    iret


;--------------------------安装8号中断----------------------------
install_int8:
    proc_save

    mov al,34h   ; 设控制字值 
    out 43h,al   ; 写控制字到控制字寄存器 
    mov ax,0ffffh ; 中断时间设置
    out 40h,al   ; 写计数器 0 的低字节 
    mov al,ah    ; AL=AH 
    out 40h,al   ; 写计数器 0 的高字节
 
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [4*8+2], ax ; 设置段地址为cs
    mov word [4*8], new_int8 ; 设置偏移地址为子过程所在位置

    proc_recover
    ret
;------------------------------------------------------------------------------
new_int8:
    interrupt_save

    ; mov ax, 0x00
    ; int 16h

    ; 设置段地址
    mov ax, 0b800h
    mov ds, ax
    call dword move_name

	mov al,20h			; AL = EOI
	out 20h,al			; 发送EOI到主8529A
	out 0A0h,al			; 发送EOI到从8529A， 注释掉好像也行，为啥？

    interrupt_recover
    iret


;##############################################################################
;--------------------------------安装33号中断----------------------------------
; 安装33号中断，用于读端口的数据。
install_int33:
    proc_save

    ; 安装 int 33 主要代码
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x33*4], new_int33
    mov word [0x33*4+2], ax
    
    proc_recover
    ret

;------------------------------------------------------------------------------
; 这是新的int33，用于读端口数据
; 传递参数：ax：读的端口号
; 返回参数：ax:读取的值
new_int33:
    ;TODO:
    ; 保存用户栈指针
    ; 将栈切换到内核栈
    ; 将用户栈保存到内核栈中
    ; TODO:
    ; 系统调用不知道如何将参数传递给调用者，暂且使用ax
    proc_save

    mov dx, ax ; 取第一个参数  端口号
    in  al, dx

    proc_recover

    iret


;##############################################################################
;--------------------------------安装34号中断----------------------------------
; 安装34号中断，用于写端口的数据。

install_int34:
    proc_save

    ; 安装 int 34 主要代码
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x34*4], new_int34
    mov word [0x34*4+2], ax
    
    proc_recover

    ret

;------------------------------------------------------------------------------
; 这是新的int34，用写端口数据
; 传递参数：
    ; bx：读的端口号
; 返回值
    ; al：  写的值
new_int34:
    ;TODO:
    ; 保存用户栈指针
    ; 将栈切换到内核栈
    ; 将用户栈保存到内核栈中

    interrupt_save

    mov dx, bx ; 取第一个参数 16位端口号
    mov al, al  ; 取第二个参数 数据
    out dx, al


    interrupt_recover

    iret


;##############################################################################
;--------------------------------安装35号中断----------------------------------
; 安装35号中断，用于读取指定内存地址处的数据。

install_int35:
    proc_save
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x35*4], new_int35
    mov word [0x35*4+2], ax
    proc_recover
    ret

;------------------------------------------------------------------------------
; 安装35号中断，用于读取指定内存地址处的数据。【读一个字节】
; 传递参数：
    ; ax : 段地址
    ; bx : 偏移量
; 返回数据
    ; ax : 
new_int35:
    ;TODO:
    ; 保存用户栈指针
    ; 将栈切换到内核栈
    ; 将用户栈保存到内核栈中
    proc_save
    mov es, ax ; 取第一个参数 段地址
    mov al, [es:bx]
    proc_recover
    iret

;##############################################################################
;--------------------------------安装36号中断----------------------------------
; 安装36号中断，用于写指定内存地址处的数据。【按字节写】

install_int36:
    proc_save
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x36*4], new_int36
    mov word [0x36*4+2], ax
    proc_recover
    ret

;------------------------------------------------------------------------------
; 安装36号中断，用于写指定内存地址处的数据。
; 传递参数：
    ; ax : 段地址
    ; bx : 偏移量
    ; cx : 数据
new_int36:
    ;TODO:
    ; 保存用户栈指针
    ; 将栈切换到内核栈
    ; 将用户栈保存到内核栈中

    interrupt_save
    mov es, ax ; 取第一个参数 段地址
    mov [es:bx], cl
    interrupt_recover
    iret