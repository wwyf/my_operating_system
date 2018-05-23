---
title: 实验报告
typora-copy-images-to: figure
---


[TOC]



#  实验目的

1. 使用时钟中断实现进程切换

## 本次实验说明

在期中的时候，我打算进入保护模式，但是那时候刚好逢实验六的DDL快到了，想到我前一个实验已经做完了进程切换，实验六再做也就没意思了，于是就打算进入保护模式试一下，没想到一试就试了这么久。拖了这么久的DDL，实在是抱歉！！

在实现本次实验的过程中，由于对之前自己实验一-实验四写的代码不太满意，又看了一些关于linux内核源码的书籍，萌生了重写一遍操作系统的想法。这一次的操作系统，尽可能做到注释清晰，层次模块分明，以便于以后自己往里面增加功能。对于一些文件的组织，代码的编写思路，参考了一些linux内核源码的设计，同时经过了自己的消化整理，在自己的操作系统中实现了类似的功能。



# 启动流程说明

由于保护模式下，所有BIOS中断都不能使用，因此在进入保护前，必须将完整的内核都事先从软盘读进内核中。理解这一点，对理解下面的说明至关重要。

编写boot的代码的思路，主要来源于linux-0.11的源码与orange的源码，以下做进一步细节的说明。

TODO:流程图

1. BOOT阶段（加载所需的扇区）
   1. 将内存0x7C00-0x7DFF复制到0x90000至0x901FF处
   2. 将第2-5个扇区（Setup程序）加载到0x92000处
   3. 将一整个系统内核（0x30000字节）加载到0x10000处
   4. 跳转到Setup程序中
2. Setup程序（进入保护模式）
   1. 将原本加载到0x10000的整个系统内核复制到0x00000处。
   2. 加载段描述符，打开A20地址线，初始化8259A中断控制器
   3. 写cr0寄存器，进入保护模式
   4. 跳入地址0x00000处（系统内核中head.asm）
3. Head.asm
   1. 本文件位于系统内核前0xB000字节
   2. 初始化全局段描述符，中断描述符
   3. 开启分页机制
   4. 进入内核main()函数。

> 关于开机流程，我想最值得讨论的就是开机时代码的各种移动了。我在以前的操作系统中，受限于原来加载在0x7c00的引导程序，没能好好的利用这片空间，对于内核代码的组织，也没有向linux-0.11的组织这么清晰。linus的做法，虽然在大量的复制中耗费了比较多的时间，不过花费的时间在之后换来的是开发过程的便捷，同时还有内存空间的充分利用，我想这样的行为是值得的。

关于这一部分的代码，由于比较多汇编代码较难说明，同时这也不是在学习操作系统中的核心内容，因此不做过多说明。

# 操作系统整体设计

## 整体架构说明

1. 目前`wyf-os`内核中主要由三个模块构成：
   1. `tty`模块，
   2. `interrupt`模块，
   3. `process模块`。
2. 除去以上模块，`wyf-os`内核中还有以下两类公共函数。
   1. `common`文件夹下的公共例程，主要为字符串操作，内存复制等常用操作
   2. `protect.h`定义的公共例程，主要为保护模式下特有的设立中断门，调用门等操作

以上代码之间的关系可见下图TODO。

## 全局变量说明

在`global.h`中，描述了在本系统中用到了所有全局变量的名称

```cpp
/**
 * @brief 终端设备表，定义在tty_drv.c中
 * 
 */
extern struct tty_struct g_tty_table[1];
/**
 * @brief 页目录表，定义在head.asm中
 * 
 */
extern page_dir_entry_t g_page_dir[1024];
/**
 * @brief 中断描述符表
 * 
 * 中断描述符表，总共256项，定义在head.asm中 
 */
extern desc_table_t g_idt_table;
/**
 * @brief 全局描述符表
 * 
 * GDT，全局描述符表，定义在head.asm中
 */
extern desc_table_t g_gdt_table;
/**
 * @brief 进程控制块表
 * 
 * 定义在process.h中 
 */
extern proc_task_struct_t g_pcb_table[MAX_PROCESS_NUM]; 

/**
 * @brief 当前进程指针
 * 
 * 
 * 该为指向当前进程的进程控制块的指针
 */
extern proc_task_struct_t * g_cur_proc;

/**
 * @brief 当前进程内核栈地址
 * 
 * 
 * 用于恢复上下文
 */
extern proc_regs_t * g_cur_proc_context_stack;

```



## 标识符命名风格

在本次操作系统的开发过程中，我发现一个良好的命名风格能够很大程度的提高操作系统的开发效率。因此，我对自己的代码做出了约定。

|  标识符类型  |                   标识符规定                    |        样例        |
| :----------: | :---------------------------------------------: | :----------------: |
|    类型名    |     modulename_name_t,以模块名为首，_t结尾      | proc_tack_struct_t |
|   全局变量   | 以g_modulename_前缀开头,取global前的字符g为标志 |    g_pcb_table     |
| 模块对外接口 |        modulename_feature,以模块名做前缀        |    tty_write()     |
| 模块内部函数 |   _modulename_feature,注意下划线开头表示私有    |  _tty_queue_put()  |
|   公共例程   |                   以com_开头                    |     com_print      |

## 注释风格

整体注释风格采用`doxygen`生成文档的格式，由于该注释的标准定下来较晚，因此有部分没有这样的注释，会在后期进一步补充。

# 各模块说明


## 公共例程

###  保护模式下的描述符操作

在`include/protect.h`中写好了对全局中断向量表进行操作的工具函数，于是在内核就可以通过简单的调用`set_XXX_gate`来对中断向量表进行设置。

TODO:

### 常用操作

在`common`文件夹下，我定义了一些常用的操作，如`com_strlen`,`com_strcpy`等操作。这些操作的实现并不是重点，因此不加详细描述。

## 字符设备的设计

linux操作系统的设计者将所有设备，文本等都抽象成一个文件的形式，我们在外界调用其`write()`函数，就能够使用该设备的功能，或者向文件写入内容。在这个的基础上，linux实现了一个虚拟文件系统。虽然我还没有实现该虚拟文件系统，不过我也尽可能仿照了这种设计的哲学，将我的终端抽象成一个字符设备，外界的`com_printk`函数在往内核终端输出信息时，只需要调用终端提供的`write()`函数即可。

### 字符设备相关的数据结构

在前面提到，内核提供的字符设备tty，以`tty_struct_t`

## 中断系统的设计

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

## 进程模块的设计

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