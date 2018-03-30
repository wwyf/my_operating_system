; 内核程序
; 在mbr中已经写好了内核在软盘和在内存中的位置
; 软盘第2-5个扇区中
; 加载到内存 0x1000处
%include "../include/macro.inc"
[bits 16]
;----------------------------内核功能入口---------------------------------------
_start:
    
; 这里放的是内核加载器，负责加载在其他扇区的程序。
    mov ax, 90
    call load_com_user_program
    jmp run_com_user_program

;###############################################################################
;------------------------------常用过程-----------------------------------------
; 运行用户com程序前运行
; 效果：
; 将cs,ds,es,ss置为0x1000
; 将sp置为0x2000(相当于.com程序末尾)
run_com_user_program:
    mov ax, 0x5000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ax, 0x2000
    mov sp, ax
    jmp 0x5000:0x0000
    
;-----------------------------------------------------------
; [内核态]加载.com简单用户程序
; 固定加载到从物理地址10000h开始的内存中
; 传入参数：ax, 这里的扇区号从0开始算起，会有一个函数负责换算。
; 过程效果：从ax开始的两个扇区被加载到内存1000:0000对应位置
load_com_user_program:
    push ax
    push bx
    push cx
    push dx
    push es

    ; ax 已经准备好，不能修改
	mov bx,0x5000               ;段地址 ; 存放数据的内存基地址
	mov es,bx                ;设置段地址（不能直接mov es,段地址）
	mov bx,0 ;偏移地址; 存放数据的内存偏移地址
    mov cl, 18
    call ReadSector

    pop es
    pop dx
    pop cx
    pop bx
    pop ax
    ret

;----------------------------------------------------------------------------
; 函数名: ReadSector
; http://blog.csdn.net/littlehedgehog/article/details/2147361
;----------------------------------------------------------------------------
; 作用:
;    从第 ax 个 Sector 开始, 将 cl 个 Sector 读入 es:bx 中
;    从0开始算
ReadSector:
    ; -----------------------------------------------------------------------
    ; 怎样由扇区号求扇区在磁盘中的位置 (扇区号 -> 柱面号, 起始扇区, 磁头号)
    ; -----------------------------------------------------------------------
    ; 设扇区号为 x
    ;                           ┌ 柱面号 = y >> 1
    ;       x           ┌ 商 y ┤
    ; -------------- => ┤      └ 磁头号 = y & 1
    ;  每磁道扇区数     │
    ;                   └ 余 z => 起始扇区号 = z + 1
    push   bp
    mov    bp, sp
    sub    sp, 2            ; 辟出两个字节的堆栈区域保存要读的扇区数: byte [bp-2]

    mov    byte [bp-2], cl
    push   bx            ; 保存 bx
    mov    bl, 18    ; bl: 除数  [每磁道扇区数]
    div    bl            ; y 在 al 中, z 在 ah 中
    inc    ah            ; z ++
    mov    cl, ah            ; cl <- 起始扇区号
    mov    dh, al            ; dh <- y
    shr    al, 1            ; y >> 1 (其实是 y/BPB_NumHeads, 这里BPB_NumHeads=2)
    mov    ch, al            ; ch <- 柱面号
    and    dh, 1            ; dh & 1 = 磁头号
    pop    bx            ; 恢复 bx
    ; 至此, "柱面号, 起始扇区, 磁头号" 全部得到 ^^^^^^^^^^^^^^^^^^^^^^^^
    mov    dl, 0        ; [驱动器号 (0 表示 A 盘)]
.GoOnReading:
    mov    ah, 2            ; 读
    mov    al, byte [bp-2]        ; 读 al 个扇区
    int    13h
    jc    .GoOnReading        ; 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止
    add    sp, 2
    pop    bp
    ret
