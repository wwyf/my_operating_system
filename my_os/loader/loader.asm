%include "../include/macro.inc"
%include "../include/pm.inc"
section LOADER_ENTRY
start:
[BITS	16]

;#################################################################
; 加载内核到内存中
;#################################################################
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



;#################################################################
; 进入保护模式前的初始化工作
;#################################################################

	; 初始化段寄存器，此时cs为0x1000， 并且分配0x100的栈空间
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 0100h

	; ; 初始化 32 位代码段描述符
	xor	eax, eax 
	mov	ax, cs
	shl	eax, 4
	add	eax, LABEL_SEG_CODE32
	mov	word [LABEL_DESC_CODE32 + 2], ax
	shr	eax, 16
	mov	byte [LABEL_DESC_CODE32 + 4], al
	mov	byte [LABEL_DESC_CODE32 + 7], ah

	; 为加载 GDTR 作准备, 将0x1000写到
	xor	eax, eax
	mov	ax, ds
	shl	eax, 4
	add	eax, LABEL_GDT		; eax <- gdt 基地址
	mov	dword [GdtPtr + 2], eax	; [GdtPtr + 2] <- gdt 基地址
	; 搞清楚了，gdt界限占用两个字节，因此加个2就到基址部分了。

	; 加载 GDTR
	lgdt	[GdtPtr]

	; 关中断
	cli

	; 打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

	; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	; 真正进入保护模式
	; 此处为16位保护模式，为了保证能够修改32位的eip，必须使用dword
	jmp	dword SelectorCode32:0

;#################################################################
; 进入保护模式后先运行的32位代码
;#################################################################

[BITS	32]

LABEL_SEG_CODE32:
	mov	ax, SelectorVideo
	mov	gs, ax			; 视频段选择子(目的)

	mov	edi, (80 * 11 + 79) * 2	; 屏幕第 11 行, 第 79 列。
	mov	ah, 0Ch			; 0000: 黑底    1100: 红字
	mov	al, 'P'
	mov	[gs:edi], ax
	jmp $

SegCode32Len	equ	$ - LABEL_SEG_CODE32



;##############################################################
; 在加载器中使用的公共例程
;##############################################################
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



;##############################################################
; GDT  Table
;##############################################################
;                              段基址,       段界限     , 属性
LABEL_GDT:   	   Descriptor       0,               0, 0           ; 空描述符
LABEL_DESC_CODE32: Descriptor 0,10000h, DA_C + DA_32; 非一致代码段
LABEL_DESC_VIDEO:  Descriptor 0B8000h,          0ffffh, DA_DRW	    
; 显存首地址, 仍然是16位的，因此就不加上DA_32

; GDT 段表信息
GdtLen		equ	$ - LABEL_GDT	; GDT长度
GdtPtr		dw	GdtLen - 1	; GDT界限
			dd  0		; GDT基地址

; GDT 选择子
SelectorCode32		equ	LABEL_DESC_CODE32	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT
; END of [SECTION .gdt]
