# week3_1 开发日记

这里主要解决的是C和汇编混编成二进制文件在实模式（16位）下运行的问题。


环境：ubuntu for window （win10子系统）
工具链：nasm，gcc，ld
## 编译指令

```sh
gcc -c -m16 -o c.o t.c
nasm -f elf32 -o asm.o t.asm
ld -o test.bin -Ttext 0x10000 -m elf_i386 -T t.lds --oformat binary c.o asm.o
```
其中`t.lds`为：
```lds
SECTIONS
{
    . = 0x10000;
    .text :
    {
        *(.text)
    }
    . = 0x10500;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}
```

`t.c`为
```c
void display_char(char );
void return_point();
void display_string(char *, long);
void display_message(long);

char message[7]="test!!";

// #include<string.h>

int main(){
    int a = add(3,4);
    // int a = 6;
    display_char('@');
    for(int i = 0;i <= 1000000;i++);
    // display_string(message,a);// 调用函数的时候，会将cs和ip（32位）的push进栈中，
    display_message(a);
    return_point();
    return 0;
}

int add(int a, int b){
    return a+b;
}
```

`t.asm`为：
```x86asm
global display_char
global return_point
global display_string
global display_message

extern message
; extern user_messageLength
%macro retl 0
    db 0x66
    ret
%endmacro

bits 16
section .data

section .text

display_char:
    mov ax, 0B800h
    mov es, ax
    mov ecx, [esp+4] ; 1 argument
    mov [es:0x00], cx
    db 0x66
    ret
return_point:
    mov ah, 01h
    int 16h
    ; 不断查询键盘缓冲区的状况
    ; 若有按键，则zf为0，若无按键，则zf为1，跳回去继续查询
    jz return_point
    ; 有字符输入,从al中读取键盘输入
    mov ah, 00h
    int 16h

    cmp al, 'q' ; 如果键入q则退出
    jnz return_point
	int 40h  ; 这个是我自己在内核中实现的从用户回到内核的中断，不是bios中断，不用管。
display_string:
    mov ax, cs
    mov es, ax
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 1 		 ; 列号=0
	mov dh, 2		       ; 行号=0
	mov ecx, [esp+8] ; CX = 串长（=9）
	mov ebp, [esp+4]		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
    db 0x66
    ret
display_message:
    mov ax, cs
    mov es, ax
	mov ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov dl, 1 		 ; 列号=0
	mov dh, 2		       ; 行号=0
	mov ecx, [esp+4] ; CX = 串长（=9）
	mov ebp, message		 ; es:BP=当前串的偏移地址
	int 10h			 ; BIOS的10h功能：显示一行字符
    db 0x66
    ret
```

## gcc 参数设置
摘自gcc 文档

>These ‘-m’ switches are supported in addition to the above on x86-64 processors in 64-bit environments.   
-m32  
-m64  
-mx32  
-m16  
-miamcu  
>Generate code for a 16-bit, 32-bit or 64-bit environment. The -m32 option sets int, long, and pointer types to 32 bits, and generates code that runs on any i386 system.   
> The -m64 option sets int to 32 bits and long and pointer types to 64 bits, and generates code for the x86-64 architecture. For Darwin only the -m64 option also turns off the -fno-pic and -mdynamic-no-pic options.   
> The -mx32 option sets int, long, and pointer types to 32 bits, and generates code for the x86-64 architecture.   
>The -m16 option is the same as -m32, except for that it outputs the .code16gcc assembly directive at the beginning of the assembly output so that the binary can run in 16-bit mode.   
>The -miamcu option generates code which conforms to Intel MCU psABI. It requires the -m32 option to be turned on.


## 疑问？

1. 开机的时候，机器如何知道我的代码是使用32位写的还是16位汇编写的。
    1. 开机的时候，计算机一定是运行在16位实模式下，此时对机器代码的解析和运行都是16位的。
    1. 此时在汇编代码中，使用`[BITS 16]`显式指明代码是16位的，然后使用nasm elf32编译后仍能保持16位的代码格式，可使用`objdump -D my_user_program_t_asm.o -m i8086 > t.list`看到反汇编出正确的代码。（用i386选项会出问题，原因在于读取立即数和地址的时候都是32位读的，而此处的机器代码是按照16位的模式下写的。）
1. 在进入保护模式前，一切的代码都是16位的，那么16位的代码能否汇编呢？

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

## 标签名不用下划线？

之前老师上课说，C和汇编相互调用的时候，汇编调用C的过程，前面要加上一个下划线。这是由于TCC在编译C代码的时候会把标识符都加上一个下划线的缘故。为了能够连接上，必须保证C代码汇编后的指令的标签名一致。

在elf中，就不存在这样的问题。（nasm文档9.1.2）

## 又遇到一些问题

之前其实是在解决一个问题，一个指令，如何告诉CPU，这是一个16位的指令还是一个32的指令。
解决了一部分，对于大多数指令而言，通过在操作码前方加上66.67后缀，来想处理器说明，这是非默认位数的指令。
但是，对于call，ret呢？如何确定他们的行为？
对于gcc而言，在编译后，无论是ret还是call，都是以32位指令的形式运行，为了告诉处理器是32位的，编译器就在操作码前加上了66,67来显式指明指令是32位的。总结，gcc默认使用32位的call和ret，非默认的call和ret

对于nasm而言，它默认使用的是默认的call和ret。
想了想，可能还会有类似的行为，比如push和pop，在c程序中，我们往往会push一些数字进去，是32位的，但是如果我pop的话，可能只能pop16位的，那么这个时候我如何显式指定为32位呢？可以通过寄存器的名称显示指定。

在16位实模式下使用32位的call，是否会出问题呢？关键问题出在了哪里呢？gcc默认使用32位的ret和call，认为16位处理器看不懂，因此会加上前缀66,67；nasm使用ret和call甚至不分情况，全都是默认，认为CPU在什么状态下应该都是以默认的方式运行代码。

突然发现，在C代码前加上.code16,ret和call都变成了16位！

## 上面遇到了新问题

.code16gcc 改成 .code16之后，所有的call和ret都变成了16位，即push16位地址进栈中，ret的时候也是pop16位的地址放到ip中。

虽然这样问题不大，但是违背了gcc的函数调用约定。首先gcc其实本来就最低编译成32位的汇编，无法实现纯16位的汇编。这样一来，就有了一个隐含的前提在gcc的汇编过程中：call，push32位的地址进栈中，ret，pop32位的地址回到调用者。

虽然可以在汇编中通过修改一些常数实现正常运行，但是在C调用C过程的时候，在gcc的编译之下，仍然将这个call的过程当成先push32位地址，那样之后参数的位置都仍然是32位下应该有的样子。实际上，由于只push了16位的地址，后面的参数全都错位了。


## 解决

既然不支持retl，calll，但是又不得不使用这样的指令以便于gcc统一，那就自己定义retl，calll，将retl解释成将原来32位的地址pop出来，然后将16位的地址push进去，那么nasm生成的汇编便可以正常返回到C中，就不会导致内存泄漏。

其实还有另一个办法，如果nasm有办法将call指令和ret指令都加上前缀66,让这些指令显式地变成32位指令。有办法！
其实简单说来，就是要让我的汇编符合gcc的规范。

使用了nasm的宏指令，将ret指令前面手动加上0x66，修改为32位的指令。

## 对0x66,0x67前缀的了解

https://wiki.osdev.org/X86-64_Instruction_Encoding

终于知道了CPU是如何分辨16位指令还是32位指令了。

在自己多次汇编和反汇编之后，搞清楚了，具有0x66,0x67前缀的指令，CPU会以非默认的方式读取操作数和
地址，如果处于16位状态（如实模式），见到前缀，就会以非默认的方式（即读32位操作数）的方式读取。

例子：call指令，机器码为`C3`。 
我发现，在我使用gcc生成的16位的代码里，gcc将ret的机器代码编译成`66 C3`，并且反汇编出来显示为`retl`，说明该指令操作的数应该为32位，再对这一条指令解释得更清楚一点，就是cpu从栈中取32位的地址出来，修改eip，从而实现跳转。这就与16位的ret只取16位的地址不同。

## 对C函数调用约定的了解
https://en.wikipedia.org/wiki/X86_calling_conventions 
http://blog.csdn.net/zsjum/article/details/6117043 加深对esp，ebp的理解
https://zh.wikipedia.org/wiki/X86%E8%B0%83%E7%94%A8%E7%BA%A6%E5%AE%9A

来自wiki的理解：cdecl（C declaration）是一种来源于C语言的函数调用约定，被大多数基于X86架构的C编译器使用。

1. 参数的传递：按照参数列表，由调用者将函数参数从右向左放进堆栈中
1. 当函数被调用的时候，默认使用call  near，将32位的地址放进堆栈中。
    1. 此时esp - 4，[esp-4]为原call语句的下一句的起始字节地址。
1. 进入被调用者，被调用者将旧的EBP寄存器放进堆栈中存起来，然后初始化新的帧栈。
    1. EBP寄存器存放了一个地址
        1. 该地址往高依次是返回地址EIP，然后是第一个参数，第二个参数。。。。
        1. 该地址往低的话是被调用者自己申请的栈空间，包括自己的局部变量等。
    1. 注意这里都是32位的！低于32位的参数提升至32位，补0，高于32位的???
    1. 被调用者将返回值放在EAX中
    1. 被调用者需要恢复到的帧栈，将之前放进栈中的ebp恢复回来。
1. 调用完被调用者之后，bp，sp是不会变的，bx也是不会变的
1. 调用者负责将被调用者的参数（自己push 的自己pop）所占的栈空间清楚

## 弄清楚ld干了什么事情
一个问题，连接生成的二进制文件可能不能控制它的数据段的位置（比如我明明代码只有100多字节，连接器非要把数据段放到两千多字节）
解决方法，linker script 已实验成功，手动修改输出的二进制文件各个段的位置

https://blog.louie.lu/2016/11/06/10%E5%88%86%E9%90%98%E8%AE%80%E6%87%82-linker-scripts/

linker script ！！！

https://github.com/SeanXP/ARM-Tiny6410/blob/master/doc/lds/Linux%E4%B8%8B%E7%9A%84lds%E9%93%BE%E6%8E%A5%E8%84%9A%E6%9C%AC%E5%9F%BA%E7%A1%80.pdf

```c
SECTIONS
{
    . = 0x10000;
    .text :
    {
        *(.text)
    }
    . = 0x10500;
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}
```

然后在ld里面加上-T指令就可以使用自己的连接脚本来进行连接了。


## 问题：程序运行时，段寄存器和栈的环境是怎样的。

我的程序在运行的时候，会把cs，ss变成程序首指令地址，然后再初始化sp为0x400，进入用户程序之后，看看gcc是如何处理我的栈段的。

```
lea    0x4(%esp),%ecx ; esp+4， 给 ecx
and    $0xfffffff0,%esp ; 低四位置0，做的应该是栈的16字节对齐吧。不是必要的
pushl  -0x4(%ecx) ; 
push   %ebp
mov    %esp,%ebp
push   %ecx
sub    $0x14,%esp  ; 应该是为了程序中的某些局部变量申请空间
sub    $0x8,%esp
;后面就是我的用户程序了。
```

https://stackoverflow.com/questions/4228261/understanding-the-purpose-of-some-assembly-statements
这个问题的回答很详细。
