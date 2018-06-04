# 实验七：fork()系统调用的实现



## 1 实验目的 

## 2 实验环境

## 3 各子模块的设计

在实现fork()，exit()系统调用前，需要实现的一些子模块如下。

由于时间的缘故，一些模块实现的是简化版。

1. 内存管理与页表转换
   1. 可能只能够分配一个连续的内存空间
2. 读取硬盘文件
   1. 由于没有实现文件系统，因此在读取硬盘文件的时候采用硬编码的形式进行读取。

### 3.1 硬盘驱动的设计

TODO:流程图！！！

目前我实现的硬盘驱动具有以下接口

```c++
void hd_identify(int drive);
void hd_read(uint32_t drive, uint32_t pos, uint32_t bytes_count, void * dest);
void hd_write(uint32_t drive, uint32_t pos, uint32_t bytes_count, void * src);
```



TODO:关键问题：irq14号中断之后，我如何判断我完成了什么操作？


#### 操作硬盘的工作流程

![](https://lh3.googleusercontent.com/-6cYOCMaGW5M/WxSoAojEx3I/AAAAAAAAIJc/JYr7U6MIjloyVB_R5clvASZTWN5B0KzLwCHMYCw/s0/Acrobat_2018-06-04_10-46-27.png)

1. 往控制块寄存器0x3F6写0，发送命令。
2. 往命令块寄存器(Command Block Registers)中写入正确的值。
   1. 标志命令开始执行的关键：Command寄存器的写入！
3. 等待irq14号线被触发
4. 中断被触发后，中断程序应该根据当前的硬盘状态来返回相应的信息。
   1. 问题：如何让irq14对应的中断知道此时的命令是什么？


#### `hd_identify`函数的编写

1. 写device寄存器
2. 使用hd_cmd_out()向硬盘驱动器发送命令
   1. 标志命令开始执行的关键：Command寄存器的写入！
   1. TODO:如何得知硬盘准备完成
3. 使用read_port函数读取信息到数组中
4. 使用hd_print_identify_info()函数打印参数

![](https://lh3.googleusercontent.com/-2Xw_T158XRs/WxSm1jzReTI/AAAAAAAAIJQ/sS2seKW6HOo7kVdiIShp7qfOyxDmQfFSwCHMYCw/s0/Typora_2018-06-04_10-41-28.png)

#### 函数运行结果

![](https://lh3.googleusercontent.com/-fpzwTOspjLw/WxSmytcakTI/AAAAAAAAIJM/J1WZFjtxEHkHmZxTf1MOxTfmTaIBwKmaQCHMYCw/s0/Snipaste_2018-06-04_10-41-15.png)

函数成功读取硬盘信息。

注意到读取硬盘信息的时候，会触发irq14号对应的中断。中断之后才准备好对应的数据。而我的实现中并没有做这样的等待。

### 3.2 读取硬盘中的文件



### 3.3 简单内存管理与页表转换

### 3.4 系统调用表

## 4 进程的设计

### 4.1 进程控制块的调整

### 4.2 进程转移状态图

### 4.3 fork()系统调用的实现

### 4.4 exit()系统调用的实现。
