# 内核初始化工作

## head.asm

该文件中的kernel_head_start作为内核入口（地址为0x00000)

主要完成以下工作。

1. 设置全局描述符表
1. 设置中断描述符表
    1. 并将所有中断初始化为哑中断
1. 开启分页机制
    1. 一个临时的分页机制，暂时是线性地址==物理地址。
1. 跳入main.c程序

## main.c

1. 编写内核库
    1. 输出
    1. 
1. 制作时钟中断