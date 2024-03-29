# boot

该部分主要参考linux-0.11的设计。

## boot

这里主要为一个文件：boot.asm

### 开机引导扇区

作为开机启动的引导扇区，该部分主要完成以下工作。完成这些工作后，将跳转到位于内存地址处0x90200的setup程序。

1. 将原来位于0x7c00的引导程序（200字节大小）整个复制到0x90000。
1. 将位于第1-4个逻辑扇区的setup模块加载到0x90200处（即紧接着引导程序后面）
1. 将从第5个逻辑扇区开始的系统内核加载到0x10000处。
    1. 难点：写一个加载固定字节数的函数
1. 跳转到0x90200，执行setup程序

### setup程序

1. 将整个内核从0x10000移动到0x00000
1. 初始化全局描述符表（GDT）
1. 打开地址线A20
1. 初始化8259A中断控制器
1. 修改cr0寄存器，进入保护模式
1. 跳转到0x00000，进入32位代码（head.asm)
