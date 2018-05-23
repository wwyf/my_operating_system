%macro proc_save 0
    push ebx
    push ecx
    push edx

    push esi
    push edi
    push ebp

    push ds
    push es
    push ss
%endmacro

%macro proc_recover 0
    pop ss
    pop es
    pop ds
    
    pop ebp
    pop edi
    pop esi

    pop edx
    pop ecx
    pop ebx
%endmacro

%macro interrupt_save 0
    push ax
    push bx
    push cx
    push dx

    push si
    push di
    push bp

    push ds
    push es
    push ss
%endmacro

%macro interrupt_recover 0
    pop ss
    pop es
    pop ds
    
    pop bp
    pop di
    pop si

    pop dx
    pop cx
    pop bx
    pop ax
%endmacro