# 中断

https://www.cnblogs.com/ningskyer/articles/6482093.html

## 进程切换的思考

1. 进程表
    1. 在内核数据段中
    1. 内容
        1. 寄存器
        1. 段寄存器
1. 时钟中断实现进程切换的实现
    1. 【用户程序栈】：int指令，将flag，segment，offset push进用户栈中
        1. 用户程序当前栈段地址，偏移地址需要保存
    1. 【栈在进程表】将ss,sp切换到进程表当前项的头指针
        1. TODO:先将对应的flag，cs，ip copy到栈的底部
        1. push 用户栈段，偏移地址，段寄存器， 寄存器     
    1. 【内核栈】 直接mov就可以转移了。
        1. 进行调度程序，修改当前运行进程号（全局变量）
    1. 【栈在进程表项】使用进程号在进程表中找到对应项的首地址，修改ss,sp
        1. pop 寄存器，段寄存器，取得ss,sp
    1. 【用户程序栈】使用在进程表中的信息，mov 修改ss,sp
        1. iret 从栈中取得flag，segment，offset，去往下一个进程

### debug 新技能 get

做法
1. gcc编译的时候加-g
2. 连接生成文件的时候，再连接生成另一个可执行文件（去掉参数 --oformat binary)
3. 对这个可执行文件进行“objdump -S -m i8086 your_file_name"

得到下面的结果[debug反汇编文件](./../os/test.debug)

![](https://i.loli.net/2018/04/05/5ac513f051500.png)