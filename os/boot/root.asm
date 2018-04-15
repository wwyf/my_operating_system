; 一个文件描述符12+10+10 = 32 字节
; file :kernel.bin
    db 'kernel.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 10 ; 起始簇号
    dd 0 ; 文件大小
; file :user_test.bin
    db 'test_a.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 54 ; 起始簇号
    dd 0 ; 文件大小
; file :user_e1.bin
    db 'test_b.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 72 ; 起始簇号  2 + (108 - 38) = 72
    dd 0 ; 文件大小
; file :user_stone_1.bin
    db 'stone1.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 90 ; 起始簇号
    dd 0 ; 文件大小
; file :user_stone_2.bin
    db 'stone2.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 108 ; 起始簇号
    dd 0 ; 文件大小
; file :user_stone_3.bin
    db 'stone3.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 126 ; 起始簇号
    dd 0 ; 文件大小
; file :user_stone_4.bin
    db 'stone4.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 144 ; 起始簇号
    dd 0 ; 文件大小
; file :user_ouch.bin
    db 'test_a.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 162 ; 起始簇号
    dd 0 ; 文件大小

