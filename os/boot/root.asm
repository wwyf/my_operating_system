; file :user.bin
    ; db 'u','s','e','r','.','b','i','n',' ',' ',' ' ; 11字节的文件名
    db 'user.bin   ' ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 90 ; 起始簇号
    dd 0 ; 文件大小
; file :user.bin
    db 'userd.bin  ' ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 90 ; 起始簇号
    dd 0 ; 文件大小
; file :user.bin
    db 'userd.bin  ' ; 11字节的文件名
    db 0
    dw 0,0,0,0,0 ; 无用数据
    dw 0 ; 最后写入时间
    dw 0 ; 最后写入日期
    dw 90 ; 起始簇号
    dd 0 ; 文件大小

