%include "../include/pm.inc"

[BITS 32]
extern main
global kernel_head_start
;TODO:页目录表占位符位置
PageDirBase:
kernel_head_start:
    mov ax, SelectorNormal
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, _stack_start
    mov ax, SelectorVideo
    mov gs, ax
    ; 同步段寄存器
    call setup_gdt
    call setup_idt
    ; 安装完全局描述符表后要重新同步段寄存器
    mov ax, SelectorNormal
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, _stack_start
    mov ax, SelectorVideo
    mov gs, ax

    ; 准备跳入main
    push 0x0
    push 0x0
    push 0x0
    push SelectorKernel
    push main
    jmp SelectorZero:setup_paging
    ret

setup_gdt:
; 设置全局描述符表
    lgdt [gdt_descr]
    ret

setup_idt:
; 设置中断向量表
    cli
    lidt [idt_descr]
    ret


times 0x1000-($-$$) db 0

; TODO:页表占位符位置
; 4KB对齐
PageTblBase:
times 0x6000-($-$$) db 0


ALIGN 2
; ---------------------------------------------------------------------------

; 启动分页机制 --------------------------------------------------------------
; 其中这里做了16MB页表的映射，并且保证了线性地址和物理地址一致。
setup_paging:
	mov	ecx, 0x4	; 此时 ecx 为页表的个数，也即 PDE 应该的个数
	mov	[PageTableNumber], ecx	; TODO:暂存页表个数 

	; 为简化处理, 所有线性地址对应相等的物理地址. 并且不考虑内存空洞.

	; 首先初始化页目录
	mov	ax, SelectorNormal
	mov	es, ax
	mov	edi, PageDirBase	; TODO:此页目录表首地址为 PageDirBase,即0
	xor	eax, eax
	mov	eax, 0 | PG_P  | PG_USU | PG_RWW
    add eax, PageTblBase
.1:
	stosd  ; 将eax存入es:di, 并将di+4
	add	eax, 0x1000		; 为了简化, 所有页表在内存中是连续的.
	loop	.1

	; 再初始化所有页表
	mov	eax, [PageTableNumber]	; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase	; 此段首地址为 PageTblBase，即0x1000-0x5000
	xor	eax, eax
	mov	eax, 0 | PG_P  | PG_USU | PG_RWW
.2:
	stosd
	add	eax, 0x1000		; 每一页指向 4K 的空间
	loop	.2

	mov	eax, PageDirBase
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .3
.3:
	nop
	ret
; 分页机制启动完毕 跳入main--------------------------------------------------------

PageTableNumber dw 4



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


times 0x6F00-($-$$) db 0
; TODO:这里是临时的栈段！
_stack_start:

times 0x7000-($-$$) db 0


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


times 0x9000-($-$$) db 0


ALIGN	32
_gdt:
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; 这里是gdt表。一项8字节，总共放256项 TODO:固定内存地址
;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;                              段基址,       段界限     , 属性
LABEL_GDT:	        Descriptor       0,               0, 0           ; 空描述符
LABEL_DESC_NORMAL:	Descriptor	     0,         0fffffh, DA_DRW 	; Normal 描述符
LABEL_DESC_ZERO:    Descriptor       0,         0fffffh, DA_C + DA_32; 非一致代码段
LABEL_DESC_KERNEL:  Descriptor       0,         0fffffh, DA_C + DA_32; 非一致代码段
LABEL_DESC_VIDEO:   Descriptor 0B8000h,          0ffffh, DA_DRW	     ; 显存首地址


; GDT 选择子
SelectorNormal		equ	LABEL_DESC_NORMAL	- LABEL_GDT
SelectorZero		equ	LABEL_DESC_ZERO	- LABEL_GDT
SelectorKernel		equ	LABEL_DESC_KERNEL	- LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO	- LABEL_GDT


times 0xb000-($-$$) db 0
