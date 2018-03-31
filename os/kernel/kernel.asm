; 内核程序
; 在mbr中已经写好了内核在软盘和在内存中的位置
; 软盘第2-5个扇区中
; 加载到内存 0x1000处
%include "../include/macro.inc"
extern system_call
extern cstart
extern tty
[bits 16]
;----------------------------内核功能入口---------------------------------------
_start:
    call install_int40
    call dword cstart
start_tty:
    call dword tty
    mov ax, 0x02
    int 0x40
    jmp $
; 这里放的是内核加载器，负责加载在其他扇区的程序。


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
    mov word [0x40*4], new_int40
    mov word [0x40*4+2], ax
    
    pop ds
    pop bx
    pop ax
    ret

; 这是新的int40，用于调用系统调用
; 使用ax索引中断
; 每一个项是16位+16位 
new_int40:

    cmp ah, 0x4c
    je .return_kernel

    mov bl, ah
    xor ax, ax
    mov al, 0x2
    mul bl
    mov si, ax
    mov bx, system_call
    call dword [bx + si] ; 注意这个call是32位的。
    iret

 .return_kernel:
    pop cx
    pop cx
    pop cx

    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x2000

    push cx
    push 0x1000
    push start_tty
    iret