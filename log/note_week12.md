---
typora-copy-images-to: figure
---

# 工作拆解

1. 中断系统的设计
2. 进程结构的设计
3. 进程切换



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
handle_irq(uint8_t n){
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






## 系统调用的设计

参考资料：
linux系统调用列表
https://linux.die.net/man/2/syscalls
https://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/appendix.html
https://www.ibm.com/developerworks/cn/linux/l-system-calls/
http://fpc-mips.sourceforge.net/LinuxSyscall_cn.html
https://linux.die.net/man/2/intro