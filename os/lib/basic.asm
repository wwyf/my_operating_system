%include "../include/macro.inc"

global write_port_byte
global read_port_byte
global write_memory_word
global write_memory_byte
global read_memory_word
global trigger_interrupt_int40
global return_from_interrupt

[bits 16]

;[未测试] 对16位端口写入8位的值
; void write_port_byte(u8 data, u16 port_number);
write_port_byte:
    push ebp
    mov ebp, esp
    push ax
    push dx
    ;enter

    mov dx, [ebp+8] ; 取第一个参数 16位端口号
    mov al, [ebp+12]  ; 取第二个参数 数据
    out dx, al

    ;laeve
    pop dx
    pop ax
    mov esp, ebp
    pop ebp 
    retl


;[未测试] 从8位端口读出8位的值
; u8 read_port_byte(u16 port_number);
read_port_byte:
    push ebp
    mov ebp, esp
    push dx
    ;enter

    mov dx, [ebp+8]  ; 取第一个参数  端口号
    in  al, dx

    ;laeve
    pop dx
    mov esp, ebp
    pop ebp
    retl


;[已测试] 对指定地址写入一字节
; void write_memory_word(u32 address, u16 data);
write_memory_word:
    push ebp
    mov ebp, esp
    push ax
    push bx
    push dx
    push ds
    ;enter

    mov bx, [ebp+8] ; 地址低16位，作为偏移量
    mov ax, [ebp+10] ; 地址高16位
    shl ax, 12       ;左移12位，作为段地址
    mov ds, ax
    mov dx, [ebp+12]
    mov [ds:bx], dx

    ;laeve
    pop ds
    pop dx
    pop bx
    pop ax
    mov esp, ebp
    pop ebp
    retl

;[已测试] 对指定地址写入一字节
; void write_memory_byte(u32 address, u8 data);
write_memory_byte:
    push ebp
    mov ebp, esp
    push ax
    push bx
    push dx
    push ds
    ;enter

    mov bx, [ebp+8] ; 地址低16位，作为偏移量
    mov ax, [ebp+10] ; 地址高16位
    shl ax, 12       ;左移12位，作为段地址
    mov ds, ax
    mov dl, [ebp+12]
    mov [ds:bx], dl

    ;laeve
    pop ds
    pop dx
    pop bx
    pop ax
    mov esp, ebp
    pop ebp
    retl

; read_memory:

;[未测试] 从指定地址读取16位的数据
; u16 read_memory_word(u32 address);
read_memory_word:
    push ebp
    mov ebp, esp
    push ax
    push bx
    push dx
    push ds
    ;enter

    mov bx, [ebp+8] ; 地址低16位，作为偏移量
    mov ax, [ebp+10] ; 地址高16位
    shl ax, 12       ;左移12位，作为段地址
    mov ds, ax
    mov ax, [ds:bx]

    ;laeve
    pop ds
    pop dx
    pop bx
    pop ax
    mov esp, ebp
    pop ebp
    retl

; 触发中断，在用户程序中直接调用
;void trigger_interrupt(u8 n);
trigger_interrupt_int40:
    ; 由于没有机会return，为了确保不发生内存泄漏，先把push进来的地址pop掉。
    ; 错了，有机会return的
    ; mov al, [ebp+8]
    int 0x40
    retl

; 从C语言中写的中断处理例程正常返回，且保证栈的正确性
; void return_from_interrupt();
return_from_interrupt:
    ; 当运行到这里的时候，栈的状态还差一点就和call前的一样
    ; 进入中断处理例程之后， 正确的标志寄存器，段地址和偏移地址已经在栈中
    ; 确保中断处理例程不修改栈后，此时在进入return这个函数，栈中多了32位的地址，pop掉然后使用iret就能够正常返回了。
    pop ecx
    iret
