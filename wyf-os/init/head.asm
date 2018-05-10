%include "../include/pm.inc"

[BITS 32]
extern main
global kernel_head_start

kernel_head_start:
    mov ax, SelectorNormal
    mov ds, ax
    mov es, ax
    mov ss, ax
    call setup_gdt
    ; 安装完全局描述符表后要重新同步段寄存器
    mov ax, SelectorNormal
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ax, SelectorVideo
    mov gs, ax
    call setup_idt
    call setup_paging
    push 0x0
    push 0x0
    push 0x0
    push SelectorKernel
    push main
    ret

setup_gdt:
; 设置全局描述符表
    lgdt [gdt_descr]
    ret

setup_idt:
; 设置中断向量表
    lidt [idt_descr]
    ret

setup_paging:
    nop
    ret



; int handler ---------------------------------------------------------------
_ClockHandler:
ClockHandler	equ	_ClockHandler - $$
	inc	byte [gs:((80 * 0 + 70) * 2)]	; 屏幕第 0 行, 第 70 列。
	mov	al, 20h
	out	20h, al				; 发送 EOI
	iretd

_UserIntHandler:
UserIntHandler	equ	_UserIntHandler - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, 'I'
	mov	[gs:((80 * 0 + 70) * 2)], ax	; 屏幕第 0 行, 第 70 列。
	iretd

_SpuriousHandler:
SpuriousHandler	equ	_SpuriousHandler - $$
	mov	ah, 0Ch				; 0000: 黑底    1100: 红字
	mov	al, '!'
	mov	[gs:((80 * 0 + 75) * 2)], ax	; 屏幕第 0 行, 第 75 列。
	jmp	$
	iretd



idt_descr dw 256*8 - 1
            dd _idt-$$
gdt_descr dw 256*8 - 1
            dd _gdt-$$


ALIGN	32
_idt:
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 这里是idt表。一项8字节，总共放256项 TODO:固定内存地址
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 门                        目标选择子,            偏移, DCount, 属性
%rep 32
		Gate	SelectorZero, SpuriousHandler,      0, DA_386IGate
%endrep
.020h:	Gate	SelectorZero,    ClockHandler,      0, DA_386IGate
%rep 95
		Gate	SelectorZero, SpuriousHandler,      0, DA_386IGate
%endrep
.080h:	Gate	SelectorZero,  UserIntHandler,      0, DA_386IGate
%rep 127
		Gate	SelectorZero, SpuriousHandler,      0, DA_386IGate
%endrep


ALIGN	32
_gdt:
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 这里是gdt表。一项8字节，总共放256项 TODO:固定内存地址
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;                              段基址,       段界限     , 属性
LABEL_GDT:	        Descriptor       0,               0, 0           ; 空描述符
LABEL_DESC_NORMAL:	Descriptor	     0,          0ffffh, DA_DRW 	; Normal 描述符
LABEL_DESC_ZERO:    Descriptor       0,          0ffffh, DA_C + DA_32; 非一致代码段
LABEL_DESC_KERNEL:  Descriptor       0,          0ffffh, DA_C + DA_32; 非一致代码段
LABEL_DESC_VIDEO:   Descriptor 0B8000h,          0ffffh, DA_DRW	     ; 显存首地址


; GDT 选择子
SelectorNormal		equ	LABEL_DESC_NORMAL	- LABEL_GDT
SelectorZero		equ	LABEL_DESC_ZERO	- LABEL_GDT
SelectorKernel		equ	LABEL_DESC_KERNEL	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT



