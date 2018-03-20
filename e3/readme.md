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


## 疑问？

1. 开机的时候，机器如何知道我的代码是使用32位写的还是16位汇编写的。
    1. 开机的时候，计算机一定是运行在16位实模式下，此时对机器代码的解析和运行都是16位的。
    1. 此时在汇编代码中，使用`[BITS 16]`显式指明代码是16位的，然后使用nasm elf32编译后仍能保持16位的代码格式，可使用`objdump -D my_user_program_t_asm.o -m i8086 > t.list`看到反汇编出正确的代码。（用i386选项会出问题，原因在于读取立即数和地址的时候都是32位读的，而此处的机器代码是按照16位的模式下写的。）
1. 在进入保护模式前，一切的代码都是16位的，那么16位的代码能否汇编呢？
    1.

gcc将代码编译成16位汇编？？
不可能的

考虑写32位汇编