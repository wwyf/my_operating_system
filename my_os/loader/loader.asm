%include "../include/macro.inc"
%include "../include/pm.inc"
section LOADER_ENTRY
start:
[BITS	16]

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
; END of [SECTION .s32]

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
