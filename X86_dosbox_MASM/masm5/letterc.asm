assume cs:code

data segment
    db 'asdfs as9234 asdf',0
data ends

code segment
start:
    mov ax, data
    mov ds, ax
    
    mov si, 0
    call letterc
    
    mov ax, 4c00h
    int 21h 
    
    ; arg: the first address of string (si)
letterc:
    push cx
    push ax
    
letterc_change:                                   
    mov cl, [si]           ; if the letter is 0, then exit the program
    mov ch, 0
    jcxz exit_letterc
    
    mov al, [si]           ; read the charecter and save in reg al
    
    cmp al, 97             ; if small than 97'a' or bigger than 122'z' then read next letter
    jb next_letter
    cmp al, 122
    ja next_letter
    
    and al, 11011111b      ; change into ด๓ะด
    
    mov [si], al           ; write into memory
        
next_letter:                   
    inc si
    jmp letterc_change

exit_letterc:
    pop ax    
    pop cx 
    ret
    
    
    
    
    
code ends
   
end start