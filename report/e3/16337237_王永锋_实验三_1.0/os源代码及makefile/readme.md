# 操作系统

这里对这个操作系统的各个模块做了具体的介绍。

[toc]

## 总体情况

|模块|软盘位置(磁头,柱面,扇区号):逻辑扇区号,扇区数量|内存位置|功能说明|
|-|-|-|-|
|boot.bin|(0,0,1):0,1|0x7c00-0x7e00|在软盘中搜索loader.bin，并加载到内存0x0800:0x0000处|
|kernel.bin|(0,2,1):72,16|0x10000-0x2FFFF|在内核下，初始化各种东西后，自动加载tty|
|tty.bin|(1,2,1):90,10|0x30000-0x34FFF|显示终端，调用各种功能|
|fsystem.bin|(?,?,?):?,4|||在内存中，作为访问文件的接口|
<!-- |loader.bin|(0,2,1):72,4|0x8000-0xFFFF|在软盘中搜索kernel.bin，并加载到内存0x1000:0x0000处| -->

## kernel.bin

0x1000:0x0000

直接跳到tty

（讲道理应该是要调用文件系统，寻找tty.bin,然后加载进内存指定位置后再跳到tty）

## tty.bin 

### 驱动机制

tty使用事件驱动型机制，对键盘输入进行轮询，一旦有输入，进行处理，若无输入，不断循环。

1. 更新显示内容
1. 判断键盘是否有输入
1. 读取当前键盘输入
    1. 当输入可打印字符时(通过`isprint`函数判断），修改显示缓冲区，更新开头索引。
    1. 当enter时,将输入缓冲区内容打包送往parser，由parser执行对应指令
    1. 回到循环开头“更新显示内容”

### 需要维护的量

主要维护以下量：

1. 输入缓冲区，输入缓冲区与显示内容异步更新。

## fsystem.bin

我的想法是始终将这个放到内存的某一个区域（如0x1000-0x2FFF）
内存中有一块作为缓冲区 0x3000-0x4000

1. 给寄存器传递一些参数，然后调用中断，通过某个寄存器索引到该项调用
1. 参数类型
    1. file_name
    1. es:bx
1. 根据参数的不同，提供以下功能
    1. error_code load_com_file(file_name, ex:bx) {0 if normal，1 if not found, 2 if file_header corrupted}
    1. init_kernel()
1. 有以下子过程
    1. 转换：逻辑扇区号->(磁头号，柱面号，扇区号)
    1. 起始簇号 search_file(file_name)  加载跟目录区到缓冲区，在根目录区搜索条目，找到后，返回指定偏移量处的起始簇号
    1. void load_file(起始簇号，es:bx)  {根据簇号，加载指定fat表所在扇区（若同一个扇区就不加载），加载指定簇，获取下一个簇，更新bx} loop
    1. TODO:删除文件，（在目录区删除指定条目）
    1. TODO:根据参数，将指定格式的文件加载
    1. TODO:分配空闲簇号，根据es:bx, length两个参数，将内存内容写到扇区中，并更新条目

