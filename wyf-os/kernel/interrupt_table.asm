extern interrupt_handler
global interrupt_table
[bits 32]

interrupt_table:
%assign i 0
%rep 256
push i
jmp interrupt_handler
nop
%assign i i+1
%endrep
