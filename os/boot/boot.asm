;程序源代码（myos1.asm）
; org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行
%include "../include/macro.inc"
section my_mbr vstart=0x7c00

jmp short LABEL_START		; Start to boot.
nop				; 这个 nop 不可少

; 下面是 FAT12 磁盘的头
BS_OEMName	DB 'WYF     '	; OEM String, 必须 8 个字节
BPB_BytsPerSec	DW 512		; 每扇区字节数
BPB_SecPerClus	DB 1		; 每簇多少扇区
BPB_RsvdSecCnt	DW 1		; Boot 记录占用多少扇区
BPB_NumFATs	DB 2		; 共有多少 FAT 表
BPB_RootEntCnt	DW ROOTENTCNT		; 根目录文件数最大值
BPB_TotSec16	DW 2880		; 逻辑扇区总数
BPB_Media	DB 0xF0		; 媒体描述符
BPB_FATSz16	DW 9		; 每FAT扇区数
BPB_SecPerTrk	DW 18		; 每磁道扇区数
BPB_NumHeads	DW 2		; 磁头数(面数)
BPB_HiddSec	DD 0		; 隐藏扇区数
BPB_TotSec32	DD 0		; 如果 wTotalSectorCount 是 0 由这个值记录扇区数
BS_DrvNum	DB 0		; 中断 13 的驱动器号
BS_Reserved1	DB 0		; 未使用
BS_BootSig	DB 29h		; 扩展引导标记 (29h)
BS_VolID	DD 0		; 卷序列号
BS_VolLab	DB 'MYOS'; 卷标, 必须 11 个字节
BS_FileSysType	DB 'FAT12   '	; 文件系统类型, 必须 8个字节  

LABEL_START:
	mov ax, cs	       ; 置其他段寄存器值与CS相同
	mov ds, ax	       ; 数据段
	mov es, ax		 ; 置ES=DS
    ; 显示字符串
    ; 此时es 就是0，与cs相同
	mov eax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov ebx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0			 ; 列号=0
	mov dh, 0		       ; 行号=0
	mov cx, MessageLength  ; CX = 串长（=9）
	mov bp, Message		 ; BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符

LoadnEx:
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
	;  TODO:加载器的地址
	mov ax,0x0800  ;段地址 ; 存放数据的内存基地址
	mov es,ax                ;设置段地址（不能直接mov es,段地址）
	mov bx,0 ;偏移地址; 存放数据的内存偏移地址
	mov ah,2                 ; 功能号
	mov al,5                ;扇区数
	mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
	mov dh,0                 ;磁头号 ; 起始编号为0
	mov ch,1                 ;柱面号 ; 起始编号为0
	mov cl,5                ;起始扇区号 ; 起始编号为1
	int 13H ;                调用读磁盘BIOS的13h功能
	; 系统内核已加载到指定内存区域中	
; LoadnEx2:
;      ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
; 	mov ax,user_program_segment ;段地址 ; 存放数据的内存基地址
; 	mov es,ax                ;设置段地址（不能直接mov es,段地址）
; 	mov bx,0 ;偏移地址; 存放数据的内存偏移地址
; 	mov ah,2                 ; 功能号
; 	mov al,18                ;扇区数
; 	mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
; 	mov dh,1                 ;磁头号 ; 起始编号为0
; 	mov ch,2                 ;柱面号 ; 起始编号为0
; 	mov cl,1                 ;起始扇区号 ; 起始编号为1
; 	int 13H ;                调用读磁盘BIOS的13h功能
; 	; 系统内核已加载到指定内存区域中

; 转入加载器
inkernel:
; TODO: 加载器段地址常量
    mov ax, 0800h
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 2000h
	mov bp, sp
	; TODO: 加载器常量
	jmp 0x0800:0x0000
Message:
	db 'Hello, MyOs is loading system core.'
MessageLength  equ ($-Message)
times 510-($-$$) db 0
db 0x55,0xaa





