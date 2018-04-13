; file :user_test.bin
    db 'u_test.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 54 ; 起始簇号
    dd 0 ; 文件大小
; file :user_e1.bin
    db 'u_e1.bin  ',0 ; 11字节的文件名
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

