# 放一些笔记

对程序的编译，连接的理解。

linux平台下的可执行文件格式。（coff，elf）

1. 相对可执行文件有三个重要的概念：编译（compile）、连接（link，也可称为链接、联接）、加载（load）。源程序文件被编译成目标文件，多个目标文件被连接成一个最终的可执行文件，可执行文件被加载到内存中运行。
1. 编译过程就不多说了
1. 连接
1. 加载
    1. 


## elf 文件格式

ELF header(ELF头部)
Program header table(程序头表)
Segment1（段1）
Segment2（段2）
………
Sengmentn（段n）
Setion header table(节头表，可选)


## gcc 参数设置
摘自gcc 文档
```
These ‘-m’ switches are supported in addition to the above on x86-64 processors in 64-bit environments.

-m32
-m64
-mx32
-m16
-miamcu
Generate code for a 16-bit, 32-bit or 64-bit environment. The -m32 option sets int, long, and pointer types to 32 bits, and generates code that runs on any i386 system.

The -m64 option sets int to 32 bits and long and pointer types to 64 bits, and generates code for the x86-64 architecture. For Darwin only the -m64 option also turns off the -fno-pic and -mdynamic-no-pic options.

The -mx32 option sets int, long, and pointer types to 32 bits, and generates code for the x86-64 architecture.

The -m16 option is the same as -m32, except for that it outputs the .code16gcc assembly directive at the beginning of the assembly output so that the binary can run in 16-bit mode.

The -miamcu option generates code which conforms to Intel MCU psABI. It requires the -m32 option to be turned on.
```


## 实验三思路

开始尝试使用汇编和C交叉编译，连接成新文件