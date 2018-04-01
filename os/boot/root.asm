; file :user.bin
    db 'user.bin  ',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 0 ; 起始簇号
    dd 0 ; 文件大小
; file :user.bin
    db 'u_test.bin',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 54 ; 起始簇号
    dd 0 ; 文件大小
; file :user.bin
    db 'us2rd.bin ',0 ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 0 ; 起始簇号
    dd 0 ; 文件大小

