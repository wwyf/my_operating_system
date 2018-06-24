%include "../include/macro.inc"


INT_M_CTL	equ	0x20	; I/O port for interrupt controller         <Master>
INT_M_CTLMASK	equ	0x21	; setting bits in this port disables ints   <Master>
INT_S_CTL	equ	0xA0	; I/O port for second interrupt controller  <Slave>
INT_S_CTLMASK	equ	0xA1	; setting bits in this port disables ints   <Slave>


global _basic_outb
global _basic_inb
global _basic_cli
global _basic_sti
global _basic_port_read
global _basic_port_write
global _basic_enable_irq

[bits 32]

;对16位端口写入8位的值
; void _basic_outb(uint8_t data, uint16_t port_number);
_basic_outb:
    push ebp
    mov ebp, esp
    proc_save

    mov dx, [ebp+8] ; 取第一个参数 16位端口号
    mov al, [ebp+12]  ; 取第二个参数 数据
    out dx, al

    proc_recover
    mov esp, ebp
    pop ebp 
    ret


;从8位端口读出8位的值
; uint8_t _basic_inb(uint16_t  port_number);
_basic_inb:
    push ebp
    mov ebp, esp
    proc_save
    ;enter

    mov dx, [ebp+8]  ; 取第一个参数  端口号
    in  al, dx

    ;laeve
    proc_recover
    mov esp, ebp
    pop ebp 
    ret


; ========================================================================
;                  void _basic_port_read(u16 port, void* buf, int n);
; NOTICE:隐含了es要与ds相同的细节
; ========================================================================
_basic_port_read:
	mov	edx, [esp + 4]		; port
	mov	edi, [esp + 4 + 4]	; buf
	mov	ecx, [esp + 4 + 4 + 4]	; n
	shr	ecx, 1
	cld
	rep	insw
	ret

; ========================================================================
;                  void _basic_port_write(u16 port, void* buf, int n);
; NOTICE:隐含了es要与ds相同的细节
; ========================================================================
_basic_port_write:
	mov	edx, [esp + 4]		; port
	mov	esi, [esp + 4 + 4]	; buf
	mov	ecx, [esp + 4 + 4 + 4]	; n
	shr	ecx, 1
	cld
	rep	outsw
	ret


;打开中断
_basic_sti:
    sti
    ret

;关闭中断
_basic_cli:
    cli
    ret


; ========================================================================
;		   void _basic_enable_irq(int irq);
; ========================================================================
; Enable an interrupt request line by clearing an 8259 bit.
; Equivalent code:
;	if(irq < 8){
;		out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << irq));
;	}
;	else{
;		out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << irq));
;	}
;
_basic_enable_irq:
	mov	ecx, [esp + 4]		; irq
	pushf
	cli
	mov	ah, ~1
	rol	ah, cl			; ah = ~(1 << (irq % 8))
	cmp	cl, 8
	jae	enable_8		; enable irq >= 8 at the slave 8259
enable_0:
	in	al, INT_M_CTLMASK
	and	al, ah
	out	INT_M_CTLMASK, al	; clear bit at master 8259
	popf
	ret
enable_8:
	in	al, INT_S_CTLMASK
	and	al, ah
	out	INT_S_CTLMASK, al	; clear bit at slave 8259
	popf
	ret
