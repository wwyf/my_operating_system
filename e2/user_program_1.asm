section header vstart=0





display_chinese_name:
	xor cx, cx
	; mov ax, chinese_name
	; mov ds, ax
display_chinese_x_loop:
	mov dx, cx
	push cx
	xor cx, cx
display_chinese_y_loop:
	xor ax, ax
	pop dx      ; 为了能够在内循环里有一个与外循环相关联的量,并且不被内循环的add语句打乱,每次用到这个量的时候都从栈中拿出来
	push dx
	mov ax, dx  ; x
	mov bx, 80
	mul bx
	add ax, cx  ; y ; 会修改dx
	mov [number_char], ax 
	mov bx, 2
	mul bx
	mov bx, ax
	; mov ah, 07h ; 与默认相同
	push si 
	push bp
	mov bp, chinese_name
	mov si, [number_char]
	; add si, 30h ;经测试,bx数字正确
	; mov ax, si
	mov al, byte [bp+si]
	; mov al, 42h ; 显示空格,即将该格显示的内容清除
	mov ah, 8bh
	pop bp
	pop si
	mov [gs:bx], ax
	inc cx      
	cmp cx, 80 ; 若y等于80,则该行清空完成,转到下一行
	jz display_chinese_next_x
	jmp display_chinese_y_loop
display_chinese_next_x:
	pop cx
	inc cx
	cmp cx, 25 ; 若x等于25,则所有列清空完成
	jz display_chinese_exit
	jmp display_chinese_x_loop
display_chinese_exit:
	jmp $
	jmp $
	jmp $
	ret

datadef:	
    count dw delay
    dcount dw ddelay
    rdul db Dn_Rt         ; 向右下运动
    ; x    db 7
    ; y    db 0
    ; char db 2
	number_char dw 0
    name db 'enter C to clean the screen! By 16337237 wang yong feng'
	message db 'enter D to display my name in chinese in several seconds'
    status dw 0720h
;     number db '16337237'

chinese_name   db '@@@@@@@@@@@@@@@@@@@                @@@@#                @@         :@@       .~#'     
db '@@@@@@@@@@@@@@@@@@@                  @@@=              @@@         @@@@@@@@@@@@*'     
db '          @#                          @@               @@         @@@       @@@ '     
db '          @#                                          @@@@@@@@@  @@@       @@@  '     
db '          @#             .           ,              =@@        @@@:  @@@ @@@    '     
db '          @#             @@@@@@@@@@@@@             =@@;          $    @@@@@     '     
db '          @#             @@@@@@@@@@@@#          @ @@@@                 @@@      '     
db '          @#                        @@         $@@!@@                 =@@@@;    '     
db '          @#                        @@        @@@@   @@@@@@@@@      .@@@@@@@@@  '     
db '          @#                        @@.      @@@@    @@@@@@@@@    :@@@@.   @@@@@'    
db '          @#                        @@@     @@@:     ,  =@  .;-#@@@@@*  @@   $@@'    
db '          @#                        @@@   ,@@@          =@    :@@@@!    @$     .'     
db '@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@.   @@@@ #@@;           =@              @,      '     
db '@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@    @#@@@@@             =@              @.      '
db '          @#                 @@=    @# @@*              =@      @@#$$$$$@#$$$$#@'     
db '          @#                 @@     @#  @@         @@@@@@@@@@@@         @.      '     
db '          @#                =@@     @#  !@@        @@@@@@@@@@@@         @.      '     
db '          @#               @@@      @#    @@@           =@       @@@@@@@@@@@@@@!'             
db '          @#              @@=       @#      @@@         =@              @.      '     
db '          @#            @@@         @#        @@@@      =@   #          @.      '     
db ';;;;;;;;;;@@;;;;;;;  #@@@           @#          #@      =@ @@@ @@@@@@@@@@@@@@@@@'     
db '@@@@@@@@@@@@@@@@@@@ @@@@            @#                  $@@@@           @.      '     
db '#*!!!!!!!!!!!!!!!!! @@$             @#                  @@@@            @.      '      
db '                                    @@,                 @@,                     '    