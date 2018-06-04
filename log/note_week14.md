## `hd_identify`函数的编写

1. 写device寄存器
2. 使用hd_cmd_out()想硬盘驱动器发送命令
   1. TODO:如何得知硬盘准备完成
3. 使用read_port函数读取信息到数组中
4. 使用hd_print_identify_info()函数打印参数

![1528074897609](C:/Users/walke/AppData/Local/Temp/1528074897609.png)