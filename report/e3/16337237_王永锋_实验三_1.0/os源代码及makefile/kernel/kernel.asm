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
    call install_int40
    call install_int8
    call dword cstart
start_tty:
    call dword tty
    mov ah, 0x02
    int 0x40
    jmp $
    jmp $
; 这里放的是内核加载器，负责加载在其他扇区的程序。


;##############################################################################
;--------------------------------安装40号中断----------------------------------
; 安装40号中断，用于用户程序返回内核
install_int40:
    push ax
    push bx
    push ds

    ; 安装 int 40 主要代码
    mov ax, 0
    mov ds, ax
    mov ax, cs
    mov word [0x40*4], new_int40
    mov word [0x40*4+2], ax
    
    pop ds
    pop bx
    pop ax
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
    push ax
    push bx
    push cx
    push dx
    push ds
    push es
    push bp

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

    ; mov ax, 0b800h
    ; mov es, ax
    ; mov ax, 0730h
    ; mov [es:0x00], ax
    pop bp
    pop es
    pop ds
    pop dx
    pop cx
    pop bx
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
    push ds

    ; mov ax, 0x00
    ; int 16h

    ; 设置段地址
    mov ax, 0b800h
    mov ds, ax
    call dword move_name

	mov al,20h			; AL = EOI
	out 20h,al			; 发送EOI到主8529A
	out 0A0h,al			; 发送EOI到从8529A， 注释掉好像也行，为啥？
    pop ds
    pop bp
    pop dx
    pop cx
    pop bx
    pop ax
    pop es
    sti
    iret