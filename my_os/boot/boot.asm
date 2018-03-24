;程序源代码（myos1.asm）
; org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行
%include "../include/macro.inc"
section my_mbr vstart=0x7c00

Start:
	mov ax, cs	       ; 置其他段寄存器值与CS相同
	mov ds, ax	       ; 数据段
	mov es, ax		 ; 置ES=DS
    ; 此时es 就是0，与cs相同
;------------------------------------------
; 清屏
	mov	ax, 0600h  ; ah=6, al=0
	mov	bx, 0700h  ; Black white
	mov	cx, 0      ;  Top left: (0,0)
	mov	dx, 184fh  ; Bottom right: (79,24)
	int	10h        ; BIOS int 10h, ah=6: Initialize screen

;-----------------------------------------------
; 显示字符串
	mov eax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov ebx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0			 ; 列号=0
	mov dh, 0		       ; 行号=0
	mov cx, MessageLength  ; CX = 串长（=9）
	mov bp, Message		 ; BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
; 将(1,0,1-18)加载器加载到0x1000:0x0000 即0x10000处
LoadnEx:
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
	mov ax,loader_code_segment                ;段地址 ; 存放数据的内存基地址
	mov es,ax                ;设置段地址（不能直接mov es,段地址）
	mov bx,0 ;偏移地址; 存放数据的内存偏移地址
	mov ah,2                 ; 功能号
	mov al,18                 ;扇区数 ；
	mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
	mov dh,0                 ;磁头号 ; 起始编号为0
	mov ch,1                 ;柱面号 ; 起始编号为0
	mov cl,1                 ;起始扇区号 ; 起始编号为1
	int 13H ;                调用读磁盘BIOS的13h功能
	; 系统内核已加载到指定内存区域中
; 转入系统内核
redirect:
	; jmp $
	jmp loader_code_segment:loader_code_entry

Message:
	db 'Hello, MyOs is loading system core.'

MessageLength  equ ($-Message)
times 510-($-$$) db 0
db 0x55,0xaa

