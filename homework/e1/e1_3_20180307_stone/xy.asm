; 程序源代码（stone.asm）
; 本程序在文本方式显示器上从左边射出一个*号,以45度向右下运动，撞到边框后反射,如此类推.
;  凌应标 2014/3
;   MASM汇编格式
     Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
     Up_Rt equ 2                  ;
     Up_Lt equ 3                  ;
     Dn_Lt equ 4                  ;
     delay equ 50000					; 计时器延迟计数,用于控制画框的速度
     ddelay equ 580					; 计时器延迟计数,用于控制画框的速度
     ;.386
     ;org 100h					; 程序加载到100h，可用于生成COM
    BOOTSEG     equ 0x0000 
    DISPLAYSEG  equ 0xb800  

    org 07c00h  
    mov ax, cs  
    mov ds, ax  
 

 
    mov ah,02h                            ;读磁盘扇区  
    mov al,01h                            ;读取1个扇区  
    mov ch,00h                            ;起始磁道  
    mov cl,02h                            ;起始扇区  
    mov dh,00h                            ;磁头号  
    mov dl,00h                            ;驱动器号  
    mov bx,os                             ;存储缓冲区  
    int 13h  

_start:  
  
    ;初始化数据段，使其指向段基址0X7C0处，即Boot代码被加载的地方  
    mov     ax, BOOTSEG  
    mov     ds, ax  
  
    ;将文本显示内存段基址 放在ES中，供后面显示字符使用  
    mov     ax, DISPLAYSEG  
    mov     es, ax  
  
    mov     cx, [msglen]  
    mov     si, message  
    xor     di, di  
  
print_str:  
  
    mov     al, [si]  
    mov     [es:di], al  
    inc   si  
    inc     di  
    mov byte    [es:di], 0x42  
    inc     di  
    loop    print_str  
  
    ;jmp near $      ;死循环，程序在此处终止  
	
start:
	xor ax,ax					; AX = 0   程序加载到0000：100h才能正确执行
    mov ax,cs
	mov es,ax					; ES = 0
	mov ds,ax					; DS = CS
	mov es,ax					; ES = CS
	mov ax,0B800h				; 文本窗口显存起始地址
	mov es,ax					; GS = B800h
	mov byte[rdul],1
	mov word[x],8
	mov word[y],0
	mov word[count],delay
	mov word[dcount],ddelay
    mov byte[char],'A'
loop1:
	dec word[count]				; 递减计数变量
	jnz loop1					; >0：跳转;
	mov word[count],delay
	dec word[dcount]				; 递减计数变量
      jnz loop1
	mov word[count],delay
	mov word[dcount],ddelay

      mov al,1
      cmp al,byte[rdul]
	jz  DnRt
      mov al,2
      cmp al,byte[rdul]
	jz  UpRt
      mov al,3
      cmp al,byte[rdul]
	jz  UpLt
      mov al,4
      cmp al,byte[rdul]
	jz  DnLt
      jmp $	

DnRt:
	inc word[x]
	inc word[y]
	mov bx,word[x]
	mov ax,25
	sub ax,bx
      jz  dr2ur
	mov bx,word[y]
	mov ax,80
	sub ax,bx
      jz  dr2dl
	jmp show
dr2ur:
      mov word[x],23
      mov byte[rdul],Up_Rt	
      jmp show
dr2dl:
      mov word[y],78
      mov byte[rdul],Dn_Lt	
      jmp show

UpRt:
	dec word[x]
	inc word[y]
	mov bx,word[y]
	mov ax,80
	sub ax,bx
      jz  ur2ul
	mov bx,word[x]
	mov ax,-1
	sub ax,bx
      jz  ur2dr
	jmp show
ur2ul:
      mov word[y],78
      mov byte[rdul],Up_Lt	
      jmp show
ur2dr:
      mov word[x],1
      mov byte[rdul],Dn_Rt	
      jmp show

	
	
UpLt:
	dec word[x]
	dec word[y]
	mov bx,word[x]
	mov ax,-1
	sub ax,bx
      jz  ul2dl
	mov bx,word[y]
	mov ax,-1
	sub ax,bx
      jz  ul2ur
	jmp show

ul2dl:
      mov word[x],1
      mov byte[rdul],Dn_Lt	
      jmp show
ul2ur:
      mov word[y],1
      mov byte[rdul],Up_Rt	
      jmp show

	
	
DnLt:
	inc word[x]
	dec word[y]
	mov bx,word[y]
	mov ax,-1
	sub ax,bx
      jz  dl2dr
	mov bx,word[x]
	mov ax,25
	sub ax,bx
      jz  dl2ul
	jmp show

dl2dr:
      mov word[y],1
      mov byte[rdul],Dn_Rt	
      jmp show
	
dl2ul:
      mov word[x],23
      mov byte[rdul],Up_Lt	
      jmp show
	

	
	

	
	message     db "16337266 xu yuan", 13, 10  
    msglen      dw $ - message 
    count dw delay
    dcount dw ddelay
    rdul db Dn_Rt         ; 向右下运动
    x    dw 7
    y    dw 0
    char db 'A'
	times 510-($-$$) db 0  
    dw  0xaa55 
	
os:  
    call os_say_hello  
    jmp $  
os_say_hello:  
    mov ax,os_message  
    mov bp,ax  
    mov cx,os_message_length  
    mov ax,01301h  
    mov bx,000eh  
    mov dx,1000h  
    int 10h  
    ret  
	
	
show:	
      xor ax,ax                 ; 计算显存地址
      mov ax,word[x]
	mov bx,80
	mul bx
	add ax,word[y]
	mov bx,2
	mul bx
	mov bx,ax
	mov ah,47h				;  0000：黑底、1111：亮白字（默认值为07h）
	mov al,byte[char]			;  AL = 显示字符值（默认值为20h=空格符）
	mov [es:bx],ax  		;  显示字符的ASCII码值
	jmp loop1
	
end:
    jmp $                   ; 停止画框，无限循环 
os_message:  
    db "[OS]os loaded"  
    db 0dh,0ah  
    db "[OS]happy using"  
os_message_length equ $-os_message  


times 1022-($-$$) db 0  
dw  0xaa55 