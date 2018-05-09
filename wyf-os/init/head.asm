%include "../include/pm.inc"

[BITS 32]
extern main
global kernel_head_start

kernel_head_start:
    push 0x0
    push 0x0
    push 0x0
    push 0x010
    push main
    ret