org 7c00h        ; ������ص�100h������������COM

; ����Դ���루stone.asm��
; ���������ı���ʽ��ʾ���ϴ�������һ��*��,��45���������˶���ײ���߿����,�������.
; ��Ӧ�� 2014/3
; MASM����ʽ

section .data
    Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
    Up_Rt equ 2                  ;
    Up_Lt equ 3                  ;
    Dn_Lt equ 4                  ;       

    delay equ 65535
    ddelay equ 65535

    ; delay equ 65535     ; ��ʱ���ӳټ���,���ڿ��ƻ�����ٶ�   
    ; ddelay equ 2000     ; ��ʱ���ӳټ���,���ڿ��ƻ�����ٶ�
    
    x dw 5
    y dw 0
    dcount dw delay
    ddcount dw ddelay
    color db 0Fh 
    count db 0 
    ten db 10
    rdul db Dn_Rt
    
section .text
start:
    mov ax,cs
    mov ds,ax         ; DS = CS       
    ;mov ax,0100h
    mov ax,0B800h       ; �ı������Դ���ʼ��ַ
    mov es,ax         ; ES = B800h


loop1: 
    mov word [es:160], 0f31h
    dec word [dcount]                ; �ݼ���������
        mov word [es:164], 0f31h
        jg loop1         ; >0����ת;
        jnz loop1
    mov word[dcount],delay
    dec word[ddcount]
        mov word [es:168], 0f31h
        jg loop1
    mov word[dcount],delay
    mov word[ddcount],ddelay

    mov word [es:320], 0f31h
    mov al,1
    cmp al,byte[rdul]
        jz DnRt
    mov al,2
    cmp al,byte[rdul]
        jz UpRt
    mov al,3
    cmp al,byte[rdul]
        jz UpLt    
    mov al,4
    cmp al,byte[rdul]
        jz DnLt
    jmp show

DnRt:
  mov word [es:480], 0f31h
  inc word [x]
  inc word [y]
  mov bx,word [x]
  mov ax,25
  sub ax,bx
        jz  dr2ur
  mov bx,word [y]
  mov ax,80
  sub ax,bx
        jz  dr2dl
  jmp show
  
dr2ur:
    ;inc byte [count]
    mov word [x],23
    mov byte[rdul],Up_Rt  
    jmp show
dr2dl:         
    ;inc byte [count]
    mov word [y],78
    mov byte[rdul],Dn_Lt  
    jmp show

UpRt:
  dec word [x]
  inc word [y]
  mov bx,word [y]
  mov ax,80
  sub ax,bx
        jz ur2ul
  mov bx,word [x]
  mov ax,0
  sub ax,bx
        jz ur2dr
  jmp show
ur2ul:
    ;inc byte [count]
    mov word [y],78
    mov byte[rdul],Up_Lt  
    jmp show
ur2dr:
    ;inc byte [count]
    mov word [x],1
    mov byte[rdul],Dn_Rt  
    jmp show

  
  
UpLt:
  dec word [x]
  dec word [y]
  mov bx,word [x]
  mov ax,0
  sub ax,bx
        jz ul2dl
  mov bx,word [y]
  mov ax,0
  sub ax,bx
        jz ul2ur
  jmp show

ul2dl:
    ;inc byte [count]
    mov word [x],1
    mov byte[rdul],Dn_Lt  
    jmp show
ul2ur:
    ;inc byte [count]
    mov word [y],1
    mov byte[rdul],Up_Rt  
    jmp show

  
  
DnLt:
  inc word [x]
  dec word [y]
  mov bx,word [y]
  mov ax,0
  sub ax,bx
        jz dl2dr
  mov bx,word [x]
  mov ax,25
  sub ax,bx
        jz dl2ul
  jmp show

dl2dr:
    ;inc byte [count]
    mov word [y],1
    mov byte[rdul],Dn_Rt  
    jmp show
  
dl2ul:        
    ;inc byte [count]
    mov word [x],23
    mov byte[rdul],Up_Lt  
    jmp show
  
show:
      ; call clearprint  
      call printnames
      inc byte [color]
      and byte [color],0Fh
      xor ax,ax                 ; �����Դ��ַ
      mov ax,word [x]
      mov bx, 80
      mul bx
      add ax,word [y]
      mov bx,2
      mul bx
      mov bx,ax
      mov ah,byte [color]        ;  0000���ڵס�1111�������֣�Ĭ��ֵΪ07h��
      mov al,'O'      ;  al = ��ʾ�ַ�ֵ��Ĭ��ֵΪ20h=�ո����
      mov [es:bx],ax      ;  ��ʾ�ַ���ASCII��ֵ
 
      ; mov bx,140
      ; mov al, byte [count]
      ; xor ah,ah
      ; div byte [ten]         
      ; push ax  
      ; mov ah,0Fh
      ; add al,30H
      ; mov word [es:bx],ax
      ; pop ax
      ; mul byte [ten]
      ; push bx
      ; mov bl, byte [count]
      ; xor bh,bh
      ; sub bx,ax
      ; mov al,bl
      ; add al,30H
      ; mov ah,0Fh 
      ; pop bx
      ; mov word [es:bx+2],ax
      
     ;   pop bx      
      jmp loop1
  
ending:
    jmp $                   ; ֹͣ��������ѭ�� 



printnames: 
    jmp real
    me db "WANG XIHUAI 16337236"
    real:        
            push ax
            push bx
            push cx
            push dx
            push es
            push bp
 
            mov al,1
            mov bh,0
            mov bl,0FH
            mov cx,20
            mov dx,0005h
            push cs
            pop es
            mov bp,me
            mov ah,13h
            int 10h
            
            pop bp
            pop es
            pop dx
            pop cx
            pop bx
            pop ax
             
            ret                       


  
; clearprint: 
;          push bx
;          push cx
;          push es
;          mov bx,0b800h  
;          mov es,bx
;          mov bx,0
;          mov cx,2000
;     clear_print_real: 
;            mov word [es:bx],5520h
;            add bx,2
;            loop clear_print_real
;          pop es
;          pop cx
;          pop bx
;          ret

;printName
;    push ds
;    push cs
;    pop ds
;    
;    PUSH AX
;    PUSH ES
;    PUSH SI
;    PUSH DI
;    PUSH CX
;    
;    MOV  AH, 0FH   ;��ȡ��ǰ��ʾ��ʽ.
;    INT  10H
;    PUSH AX
;    MOV  AX, 12H   ;���õ�����ʾ
;    INT  10H
;    MOV  AX, 0A000H ;��ʾ����ε�ַ
;    MOV  ES, AX
;    CLD
;    LEA  SI, DOT
;    MOV  DI, 0
;    MOV CX, 03h
;printAll:
;    push CX
;    MOV CX, 10h
;one:
;    MOVSW
;    ADD  DI,78     ;�൱�Ǽ���80
;    LOOP one
;    pop CX
;    ADD DI, 2
;    sub di, 16*80
;    LOOP printAll
;    POP  AX
;    
;    push cx
;    mov cx, waits
;    
;    waitloop1:
;        push cx
;        mov cx,waits2
;        
;        waitloop2:
;            loop waitloop2 
;        loop waitloop1
;        pop cx
;    
;    pop cx
;                
;    
;    MOV  AH, 00H   ;�ָ�ԭ������ʾ��ʽ.
;    INT  10H
;    
;    POP CX
;    POP DI
;    POP SI
;    POP ES
;    POP AX 
;    pop ds
;    
;    ret
;    ; ��ģ
;    DOT DB 7FH, 0FEH, 01H, 80H, 01H, 80H, 01H, 80H, 01H, 80H, 01H, 80H, 01H, 80H, 1FH, 0F8H
;        DB 1FH, 0F8H, 01H, 80H, 01H, 80H, 01H, 80H, 01H, 80H, 01H, 80H, 01H, 80H, 7FH, 0FEH
;    
;    DB 0H, 3EH, 08H, 42H, 14H, 7EH, 22H, 22H, 40H, 7EH, 1CH, 40H, 09H, 0FEH, 3DH, 2AH
;    DB 08H, 4AH, 0AH, 92H, 0CH, 22H, 00H, 42H, 00H, 12H, 00H, 0EH, 00H, 00H, 00H, 00H
;    
;    DB 40H, 88H, 21H, 00H, 12H, 0BEH, 04H, 88H, 00H, 88H, 00H, 88H, 08H, 0BEH, 78H, 0BEH
;    DB 00H, 88H, 00H, 88H, 00H, 0BEH, 00H, 88H, 08H, 88H, 38H, 88H, 70H, 0BEH, 00H, 00H
;







