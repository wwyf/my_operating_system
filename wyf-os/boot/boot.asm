%include "../include/pm.inc"
; 系统内核大小
SYSSIZE equ 0x3000
SETUPLEN equ 4				; setup 程序的扇区数量
BOOTSEG  equ 0x07c0			; bootsect 的原始段地址
INITSEG  equ 0x9000			; 将bootsect移动到该段
SETUPSEG equ 0x9020			; setup程序加载到该段
SYSSEG   equ 0x1000			; 内核加载到 0x10000 .
ENDSEG   equ SYSSEG + SYSSIZE		; where to stop loading

jmp start
sectors dw 18 ; 软盘每磁道的扇区数
[BITS 16]
start:
    mov	ax,BOOTSEG
    mov	ds,ax
    mov	ax,INITSEG
    mov	es,ax
    mov	cx,256 ; bootsect大小不超过512字节，也就256个字
    sub	si,si ; 源地址：  ds:si = 0x07c0:0x0000
    sub	di,di ; 目的地址：es:di = 0x9000:0x0000
    rep movsw
    jmp INITSEG:go
go:
    mov	ds,ax
	mov	es,ax     
	mov	ss,ax  ; 此时ax为0x9000， 同步到其他三个段中

	mov	sp,0xFF00		; 设置栈指针

load_setup:
; 下面是将位于扇区2-5的setup程序加载到内存0x90200地址处
    mov dx, 0
    mov cx, 2
    mov bx, 0x0200
    mov ax, 0x0200 + SETUPLEN
    int 0x13
    jnc ok_load_setup
    mov dx, 0x0000
    mov ax, 0x0000
    int 0x13
    jmp load_setup

ok_load_setup:
; 加载setup程序之后，将一整个系统内核加载到内存0x10000中
    mov ax, SYSSEG
    mov es, ax
    call read_kernel ; 将一整个内核读进内存es对应的段开始的位置。
    call kill_motor
    ; TODO:使用哪个根文件系统设备？

    ; 跳转到setup程序中！
    jmp SETUPSEG:0x0000 

read_kernel:
; 需要参数：es，用于确定系统内核加载到的段，并且这个段必须存放在内存地址64KB的边界开始处。
	mov ax,es
	test ax,0x0fff
die:	
    jne die			; es must be at 64kB boundary
	xor bx,bx		; bx 段内偏移 为0
rp_read:
	mov ax,es
	cmp ax,ENDSEG		; 查看是否已经加载了全部数据
	jne ok1_read        ; 如果没有加载完，继续加载
	ret
ok1_read:
; 计算当前磁道需要读取的扇区数 ，结果会放在ax中
	mov ax,[cs:sectors]
	sub ax,[cs:sread]
	mov cx,ax           ; cx:当前磁道未读扇区数
	shl cx,0x9          ; 得到读的字节数
	add cx,bx           ; cx:此次读操作后，段内写入字节数  vx:段内偏移
	jnc ok2_read
	je ok2_read         ; 不超过64KB字节，就去直接读
	xor ax,ax        
	sub ax,bx
	shr ax,0x9          ; 超过了就计算此时最多能读入的字节数
ok2_read:
	call read_track
	mov cx,ax
	add ax,[cs:sread]
	cmp ax,[cs:sectors]
	jne ok3_read
	mov ax,0x1
	sub ax,[cs:head]
	jne ok4_read
	inc word [cs:track]
ok4_read:
	mov [cs:head],ax
	xor ax,ax
ok3_read:
	mov [cs:sread],ax
	shl cx,0x9
	add bx,cx
	jnc rp_read
	mov ax,es
	add ax,0x1000
	mov es,ax
	xor bx,bx
	jmp rp_read

read_track:
; 读取当前磁道上指定开始扇区和需读扇区数（由al提供）到es:bx开始处
	push ax
	push bx
	push cx
	push dx
	mov dx,[cs:track]  ; TODO:当前磁道号 
	mov cx,[cs:sread]  ; TODO:当前磁道上已读扇区
	inc cx
	mov ch,dl
	mov dx,[cs:head]   ; TODO:当前磁头号
	mov dh,dl
	mov dl,0x0
	and dx,0x0100
	mov ah,0x2
	int 0x13
	jc bad_rt
	pop dx
	pop cx
	pop bx
	pop ax
	ret
bad_rt:	
    mov ax,0x0
	mov dx,0x0
	int 0x13
	pop dx
	pop cx
	pop bx
	pop ax
	jmp read_track

kill_motor:
	push dx
	mov dx,0x3f2
	mov al,0x0
	out dx, al ; 将al中的内容输出到dx指定的端口
	pop dx
	ret

sread dw 1+SETUPLEN ; 磁道中已读扇区数
head  dw 0          ; 当前磁头号
track dw 0          ; 当前磁道号


times 	510-($-$$)	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志

; 以下是setup部分 在2-5扇区中。

setup_start:
    mov ax, INITSEG
    mov ds, ax
    mov ah, 0x03
    xor bh, bh
    int 0x10
    mov [0x0000], dx ; 保存光标位置

    ;TODO: 获取内存的大小

    ; TODO: 获取显卡当前显示模式，及设置显示模式

    ; TODO: 取硬盘的信息，并检测是否有两个硬盘



;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 准备进入保护模式
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    cli ; 关闭中断
;----------------将整个内核从0x10000移动到0x00000---------------------
	mov	ax,0x0000
	cld			; 'direction'=0, movs moves forward
do_move:
	mov	es,ax		; destination segment
	add	ax,0x1000
	cmp	ax,0x9000
	jz	end_move
	mov	ds,ax		; source segment
	sub	di,di
	sub	si,si
	mov cx,0x8000   ; 移动64KB字节
	rep movsw
	jmp	do_move

end_move:
    ; mov ax, SETUPSEG
    ; mov ds, ax
    mov ax, INITSEG
    mov ds, ax
;----------------------加载段描述符----------------------------------
    lidt [idt_48]
    lgdt [gdt_48]

;----------------------开启A20地址线---------------------------------

	; 打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al


;----------------------初始化8259A中断控制器--------------------------

TODO:

;------------------------设置保护模式比特位--------------------------

	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 这里跳转到内核的段中。
; 真正进入保护模式
; 此处为16位保护模式，为了保证能够修改32位的eip，必须使用dword
    jmp	dword SelectorKernel:0
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 一些数据。
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
idt_48:
    dw 0
    dd 0
gdt_48:
    dw 0x800
    dd 0x90000+LABEL_GDT

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 这里是临时的gdt表。
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; GDT
;                              段基址,       段界限     , 属性
LABEL_GDT:	        Descriptor       0,               0, 0           ; 空描述符
LABEL_DESC_CODE32:  Descriptor       0,          0ffffh, DA_C + DA_32; 非一致代码段
LABEL_DESC_KERNEL:  Descriptor       0,          0ffffh, DA_C + DA_32; 非一致代码段
LABEL_DESC_VIDEO:   Descriptor 0B8000h,          0ffffh, DA_DRW	     ; 显存首地址
; GDT 结束

; GDT 选择子
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT
SelectorKernel		equ	LABEL_DESC_KERNEL	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT



