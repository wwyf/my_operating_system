;程序源代码（myos1.asm）
; org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行
section my_mbr align=16 vstart=0x7c00
my_core equ 1000h
Start:
	mov ax, cs	       ; 置其他段寄存器值与CS相同
	mov ds, ax	       ; 数据段
	mov es, ax		 ; 置ES=DS
    ; 显示字符串
    ; 此时es 就是0，与cs相同
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0			 ; 列号=0
	mov dh, 0		       ; 行号=0
	mov cx, MessageLength  ; CX = 串长（=9）
	mov bp, Message		 ; BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
LoadnEx:
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
	; mov ax,cs                ;段地址 ; 存放数据的内存基地址
	; mov es,ax                ;设置段地址（不能直接mov es,段地址）
	mov bx, my_core  ;偏移地址; 存放数据的内存偏移地址
	mov ah,2                 ; 功能号
	mov al,1                 ;扇区数
	mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
	mov dh,0                 ;磁头号 ; 起始编号为0
	mov ch,0                 ;柱面号 ; 起始编号为0
	mov cl,2                 ;起始扇区号 ; 起始编号为1
	int 13H ;                调用读磁盘BIOS的13h功能
	; 系统内核已加载到指定内存区域中
	jmp my_core
return_point:
	jmp $                      ;无限循环
Message:
	db 'Hello, MyOs is loading system core.'
MessageLength  equ ($-Message)
	times 510-($-$$) db 0
	db 0x55,0xaa


section my_user_program align=16 vstart=0x8100
core_start:
    ; 初始化内核段地址


    ; 显示内核信息
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 0 		 ; 列号=0
	mov dh, 1		       ; 行号=0
	mov cx, Core_MessageLength  ; CX = 串长（=9）
	mov bp, Core_Message		 ; BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符

Core_Message:
    db 'You are in my os core now!'
Core_MessageLength equ ($-Core_Message)
