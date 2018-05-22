---
title: 实验报告
typora-copy-images-to: figure
---


[TOC]



## 待完成

1. 进程的封装，ring3的进入
2. 进程的切换，目前的话，不同的进程都写在内核里面，然后链接在一起。
   1. 目前的问题是：内核所使用的C库与用户所使用的C库里面的函数名如果一样会怎样？可能会报错。
3. 最终效果
   1. 没有键盘，没有硬盘
   2. 不过可以看见两个用户程序交替进行，就好，并且这两个用户程序使用了由C库包装的系统调用。
4. 创建进程
   1. 给新进程分配内存空间，并且分配页表。
   2. 使用函数指针创建进程。



现在有一个很大的问题：用户程序所用C库能否和操作系统编译到一起？这要求操作系统用到的函数前面都需要加上前缀。对，这样就应该做完了。

想有一套更好的命名风格，这样子我的系统就可以做的更好了

分模块命名，某一个模块的函数带有模块前缀的命名，向内核外提供接口的就带sys前缀，公用库的函数就带com前缀。在模块间提供接口的不需要下划线开头，模块内函数需要带下划线表示该函数不会再其他模块调用。



常量使用大写，特别重要的常量使用两个下划线，并且在常量名字中说明范围，如`__KERNEL_CS`



里面使用到的各种结构体的名字，需要使用驼峰命名法。并且在类型名前加上属于的模块前缀。

变量名的话临时变量随便取，全局变量在变量名前带g前缀。



1. 内核进程的切换。
2. 用户进程的使用。



# 操作系统整体设计

## 文件组织



## 启动流程

###  引导启动 

在引导启动阶段，运行的代码主要在boot.asm

### 初始化保护模式

`head.asm`

### 初始化操作系统


# 操作系统模块说明

## 模块组织概述

## 中断系统的设计

###  编写工具函数

在`include/protect.h`中写好了对全局中断向量表进行操作的工具函数，于是在内核就可以通过简单的调用`set_XXX_gate`来对中断向量表进行设置。

### 通用中断处理例程的设计

我们先使用以下的方式对中断处理例程进行初始化，这样初始化后，就可以使用一个通用的函数对中断进行处理与分流。注意x86自带的陷阱后面会被覆盖。

```cpp
interrupt_0:
	push 0
    jmp <common_intr_handler>
    nop
interrupt_1:
	push 1
    jmp <common_intr_handler>
    nop
....
```

当中断发生后，会将中断push进内核栈中，并且跳到`common_intr_handler`中进行处理。

该函数会判断该中断的类型，并调用对应的处理例程。

```cpp
interrupt_handler(uint8_t n){
    // 索引到irq_desc的第n项，这里的n 如何对应上？
    // 调用该项对应的action
}
```

以下分两类中断进行设置

#### 陷阱门 `traps_32.h/c`

在内核启动函数中，使用`init_trap()`函数来初始化保护模式内的32种异常（或者说陷阱）

这里的函数都在文件`traps_32.c`内实现

该`init_trap()`函数内大概如此实现，**会将前面已经初始化过的一些表项覆盖掉**。

```cpp
void init_trap(){
    set_trap_gate(0,&trap_function_pointer);
    set_trap_gate(1,&trap_function_pointer);
    ....
    set_trap_gate(31,&trap_function_pointer)
}
```

具体的异常处理函数，也都放在该文件中。

#### irq线对应的中断处理例程 `interrupt.h/c`

x86一共有256个中断，除了32个intel保留的内部中断，剩下的224个中断由irq_desc数据负责处理与实现。

```cpp
struct irq_desc{
    /* 中断处理例程 */
    struct irqaction *action;
}
```

## 进程

1. 确定进程控制块的结构
2. 编写restart函数，启动一个init进程，该进程工作在ring3
3. 编写时钟中断
4. 编写一个系统调用，需要有一个完整的流程



当时钟中断发生的时候，push一个数，然后save 寄存器到 进程控制块，

```cpp
恢复的时候，从新进程的内核栈中获得上下文，然后返回。
```

1. 进程控制块，头为栈信息







## 遇到的问题记录

C语言中，传参的指针会失去大小信息

```cpp
void update_current_process_context(PT_REGS * regs){
    ...
    memcpyk((char *)regs, (char *)&current->regs, sizeof(regs));
    ...
}
```

我有一个函数一开始是这么写的，但是一直没有成功，后来打印了`sizeof(regs)`发现结果是4

![1526977750932](figure/1526977750932.png)

终于才明白过来，不能够这么写，必须使用类型名。

```cpp
void update_current_process_context(PT_REGS * regs){
    ...
    memcpyk((char *)regs, (char *)&current->regs, sizeof(PT_REGS));
    ...
}
```




## 系统调用的设计

参考资料：
linux系统调用列表
https://linux.die.net/man/2/syscalls
https://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/appendix.html
https://www.ibm.com/developerworks/cn/linux/l-system-calls/
http://fpc-mips.sourceforge.net/LinuxSyscall_cn.html
https://linux.die.net/man/2/intro