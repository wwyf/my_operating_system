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

## 总结一下

其实要想明白，为什么要C与汇编混合编程，以及现阶段遇到的关键问题。

CPU在开机进入16位实模式的时候，对机器代码的读取与执行是这样子的，对操作数和地址的读取，默认为16位；如果在操作码前加上0x66,或0x67,就能够切换模式，读取32位的操作数和地址，这个加上前缀的过程汇编器就可以完成，不需要我们操心。
既然如此，这就意味着其实只要是32位的CPU，无论是什么汇编语句都能够运行，无论是16位的汇编，还是32位的汇编，没有区别。对于设置默认模式的问题，在编译器中，可以通过[bits 16]显式指定，产生符合要求的运行在16位环境下的机器代码，当然，也可以通过[bits 32]位显式制定，产生运行在32位环境下的代码，此时若是想临时使用16位的寄存器和地址，加前缀即可，如何加前缀，通过之前显式说明的伪指令即可完成，不需要我们操心。

**既然我们说到，16位和32位的汇编指令没有区别，任何CPU都可以运行。**因此，在实模式下的CPU，我们同样可以使用32位的汇编来编写。那问题出在了哪里呢？

我们要解决的问题是使用C编写系统内核，如何做到？MBR中的内容汇编写完就算啦，但是系统内核，如果我想调用一个C过程，该如何实现？或者说，先使用C语言，写一个用户程序吧。
首先，用户程序固定加载到0x10000内存地址处，开始运行它的第一条指令



## 到底保护模式是为啥
保护模式：修改了寻址方式
A20地址线：修改了寻址空间

如果进了保护模式，不开A20地址线，设置为16位，那么：GDT表仍然可以正确设置，不过就是地址的计算有一些区别，并且不能超过FFFFF，对于这个限制，我只需要自己控制好了就行了。

如果只开A20地址线，我是不是可以任意使用？：
>http://blog.csdn.net/jessenchen/article/details/5680467
>实模式下也是可以访问1M以上的内存空间的。在CPU内部有个段描述符高速缓寄存器，寄存器内容分为三个部分：段基址(32bit)，段界限(32bit),段属性(读，写，执行，存在等)。在实模式下：段基址   ＝   Seg   *   16,   段界限   ＝   OFFFFh,   在保护模式下:   段基址   ＝   描述符基址,     段界限   ＝描述符界限。但无论在实模式下还是在保护模式下：最后得到的物理地址是一致的为:段基址   +   段偏移。   如果你想在实模式下防问1M以上的空间，那么你必须修改高速缓寄存器中段界限，但是在实模式下此值又不能修改，所以你必须先跳到保护模式下修改此值，然后再跳会实模式。只要你把段界限值修改到足够大，你就能访问相应的内存了。

为了突破1M寻址的限制，需要做4方面的工作

1. 需要做到对地址进行加减（偏移）的时候，能够突破1M：--------------------打开A20地址线
1. 需要自己设置描述符高速缓存器，突破在实模式下内置的默认界限：0xFFFF-----进入保护模式
1. 需要CPU默认读取32位地址和操作数-------------------------------------进入保护模式后，设置默认读32位
1. 需要支持存储32位的寄存器便于存储地址和数据---------------------------使用32位的汇编
