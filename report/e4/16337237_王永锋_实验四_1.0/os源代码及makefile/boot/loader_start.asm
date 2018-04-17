%include "../include/macro.inc"

global read_sector
global kernel_start
[bits 16]
;------------------------------------------------------------
;----------------------------------------------------------------------------
; 函数名: ReadSector
; http://blog.csdn.net/littlehedgehog/article/details/2147361
;----------------------------------------------------------------------------
; 作用:
;    从第 ax 个 Sector 开始, 将 cl 个 Sector 读入 es:bx 中
;    从0开始算
;                       +8      12     ebp + 16      ebp + 20      ebp + 24
; void read_sector(u16 磁头, u16 柱面, u16 扇区号,  u16 segment, u16 offset)
read_sector:
    push ebp
    mov ebp, esp
    push ax
    push bx
    push cx
    push dx
    push es
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
	mov ax,[ebp+20]  ;段地址 ; 存放数据的内存基地址
	mov es,ax                ;设置段地址（不能直接mov es,段地址）
	mov bx,[ebp+24] ;偏移地址; 存放数据的内存偏移地址
	mov ah,2                 ; 功能号
	mov al,1                ;扇区数
	mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
	mov dh,[ebp+8]                 ;磁头号 ; 起始编号为0
	mov ch,[ebp+12]                 ;柱面号 ; 起始编号为0
	mov cl,[ebp+16]                 ;起始扇区号 ; 起始编号为1
	int 13H ;                调用读磁盘BIOS的13h功能

    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    mov esp, ebp
    pop ebp
    retl

kernel_start:
    mov ax, 0x1000
    mov ds, ax
    mov es, ax
    mov ss, ax
    ; TODO: sp的值需要注意
    mov sp, 0x5000
    ; TODO:改段地址之后，进入内核
    jmp 0x1000:0x0000
    