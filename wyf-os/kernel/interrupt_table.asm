extern interrupt_handler
extern current_process_kernel_stack

global interrupt_table

[bits 32]

%macro SAVE_ALL 0
push fs
push es
push ds
push eax
push ebp
push edi
push esi
push edx
push ecx
push ebx
%endmacro

%macro RESTORE_ALL 0
pop ebx
pop ecx
pop edx
pop esi
pop edi
pop ebp
pop eax
pop ds
pop es
pop fs
%endmacro



interrupt_table:
%assign i 0
%rep 256
push i
jmp common_handler
nop
%assign i i+1
%endrep


common_handler:
    SAVE_ALL
    mov eax, esp
    push eax
    call interrupt_handler
    pop eax
    jmp ret_from_intr


ret_from_intr:
    ; TODO:获得新进程的内核栈指针
    mov esp, [current_process_kernel_stack]
    RESTORE_ALL
    add esp, 4
    iret

    