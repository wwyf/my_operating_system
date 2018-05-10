%include "../include/macro.inc"

global _basic_outb
global _basic_inb
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
