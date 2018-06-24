---
tags: #inbox
---

# 实验7：fork，exit，wait系统调用的实现



[TOC]

## 1 实验目的

1. 完善进程控制模型
2. 实现一个5状态进程模型

## 2 本实验完成的功能

通过这一个实验，我为自己实现的操作系统中增加了三个系统调用。

1. fork
2. exit
3. wait

这三个系统调用，建立在原先的进程间通信的机制上，通过向“mm"进程发送消息来实现。具体地说，就fork系统调用而言，父进程在调用fork系统调用后，操作系统能够找到一块空闲的进程控制块，将父进程的信息复制到子进程中，并且向mm进程发送消息以分配子进程的内核空间。而其他的系统调用也都类似。

实现这几个系统调用后，我编写了一个测试程序，来测试这几个系统调用的正确性，验证了自己实现的五状态进程模型。

## 3 基本原理

###  3.1 fork的实现
      1. 主要工作：向mm进程发送一个`fork`消息，主要工作由mm进程完成
      2. mm进程是这样子完成fork的
      1. 寻找到一个空闲的进程控制块（通过遍历PCB数组，寻找空闲的块就可以实现）
      2. 复制发送消息者的pcb到空闲进程控制块中
      3. 从内存池中，分配一块栈空间
      4. 复制栈到新进程的栈空间中
      5. 将子进程的状态设置为running

### 3.2 wait 的实现

对于用户的wait，我是这样子实现的：

```c
PUBLIC int user_wait(int * status)
{
	message_t msg;
	msg.type   = WAIT;

	msg_send_recv(BOTH, TASK_MM, &msg);

	*status = msg.STATUS;

	return (msg.PID == NO_TASK ? -1 : msg.PID);
}
```

这里就通过使用`msg_send_recv`系统调用，向`TASK_MM`进程发送`WAIT`类型的消息来实现wait。

具体的`do_wait`操作，在`mm/forkexit.c`中实现。

do_wait的实现可以如下说明：

1. 



### 3.3 exit的实现



## 4 遇到的问题

调用fork后，生成的子进程无法正常的从函数中返回。这个涉及到堆栈信息了。

试想这样一种情况，我在调用某一个函数后，执行了fork函数，派生出来了一个子进程，如果该子进程正常开始运作，那么他如何正确的从函数中返回呢？此时堆栈中存有的地址都是原先父进程地址空间中的地址，然而子进程的地址空间与父进程不同，因此这就导致子进程无法正确从函数中返回。

根本原因在于子进程与用户进程的地址空间不一致。如此情况，在我去设计我的fork进程的时候，便显得很困难。

不过，仔细想了一下，子进程和父进程的地址空间相同呀。没有这个问题，先试一下。



### 一个大坑

我曾出现过一个问题：子进程返回的pid值不对。

![](https://lh3.googleusercontent.com/-tSfHl6lz1rc/Wy-0ImGQiNI/AAAAAAAAIoU/CW-6Is6LLN0v39YSmd3iDEVYQgkLSLr6QCHMYCw/s0/Code_2018-06-24_23-09-21.png)

在用户的fork函数中，我使用了一个FORK消息来发送，同时该消息也兼顾接受返回值的作用。问题的出现在于，这是一个父进程中的局部变量，在父进程的局部堆栈中。当我派生出子进程后，该子进程中的堆栈存有的仍然是父进程的message绝对地址。因此返回的消息仍然在使用着父进程的内存，但是在C语言编译的过程中，该消息是使用esp来索引访问的，子进程的esp与父进程是不同的（给子进程申请了一段新的堆栈）。

因此，对于子进程而言，mm进程返回的消息复制到了父进程的内存空间中，而子进程使用相对寻址，因此子进程无法访问到mm进程返回的消息，也就是说，访问的地址是错误的，这就导致了子进程返回的pid值不对。

最根本的问题在于子进程和父进程的地址空间不一致。如果有页表的话，其实可以做到通过更换页表的方式，达到地址空间不变，而使用不同的物理内存。不过由于我目前的页表还比较简单，虽然开启了页表，但是初始化为“线性地址等于物理地址”的形式。因此这个问题我暂时还比较难解决。

我的解决方法是：将fork所使用的消息变量， 改为全局变量，这样子就避免了相对寻址导致的问题。

![](https://lh3.googleusercontent.com/-EmroEyQEifg/Wy-3ZkmNO8I/AAAAAAAAIog/bDq8zbMfkNYaMqAT7vYSMVhqNlu0dAmTgCHMYCw/s0/Snipaste_2018-06-24_23-23-18.png)

### 一个问题

![](https://lh3.googleusercontent.com/-ecPl-FhaHuc/Wy_XlM1AFqI/AAAAAAAAIos/tRlJOIKm9KsPgvtxZbPyxlSelqGgVJz7gCHMYCw/s0/qemu-system-i386_2018-06-25_01-40-34.png)

在我编写wait和exit的时候，发生了这样的问题，代码中的assert检测到了问题，及时提示消息，停止了系统的进一步运行。后来发现这个是因为我忘记了在清楚进程控制块的时候，往status打上一个已空的标记。

## 5 测试过程

我使用了这样的代码进行测试，可见`test/test.c`

```c++
/***********************************************/
/* 测试消息机制下的wait 和 exit系统调用
/***********************************************/
PRIVATE void _test_wait_exit_process(){
    com_printk("Init() is running ...\n");
    /* 下面将派生出父进程和子进程 */
	int pid = user_fork();

	if (pid != 0) { /* parent process */
		com_printk("pid(%d) : parent is running, child pid:%d\n", user_get_pid(), pid);
        int status;
        user_wait(&status);
        com_printk("pid(%d) : child has exited, exit status is %d\n", user_get_pid(), status);
	}
    
	else {	/* child process */
		com_printk("pid(%d) : child is running, pid:%d\n", user_get_pid(), user_get_pid());
        user_exit(10);
	}
    while (1){}
}
```



这部分代码做的是派生出两个父进程和子进程，然后父进程调用`user_wait`等待子进程，子进程调用`user_exit`让父进程从wait中返回，父进程从而打印出子进程的退出状态。



## 6 实验感想

