---
typora-copy-images-to: figure
---

# fork()系统调用的实现

我想，为了完成一个用户程序，我需要：



1. 转换页表，使当前页表对应的地址空间为新用户进程的地址空间
   1. 页表是原来init进程的克隆
   2. 写好缺页异常处理
2. 软盘驱动程序，可以从软盘中加载指定位置处的二进制数据到指定内存地址处
   1. 这里会使用到缺页异常来加载可用物理页，然后将程序加载到这个物理页中。
   2. 在fork中，不需要用到软盘驱动程序，在exec中才需要。
3. 读取可执行文件的头部信息，用于初始化一些寄存器，如运行指令偏移地址，eip，用户栈，等
   1. 这些信息会写入进程控制块中的相应位置
4. 跳转，运行，修改当前进程数等等。





系统调用的设计

参考资料：
linux系统调用列表
https://linux.die.net/man/2/syscalls
https://www.ibm.com/developerworks/cn/linux/kernel/syscall/part1/appendix.html
https://www.ibm.com/developerworks/cn/linux/l-system-calls/
http://fpc-mips.sourceforge.net/LinuxSyscall_cn.html
https://linux.die.net/man/2/intro