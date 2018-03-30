; %include "../include/basic.h"

global trigger_interrupt_int40
global return_from_interrupt

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
