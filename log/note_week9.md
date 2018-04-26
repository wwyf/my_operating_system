# VFS

## 接口

```c
Create(dir, dentry, mode) 在某个目录下，为与dentry 目录项相关的常规文件创建一个新的磁盘索引节点
Lookup(dir, dentry) 查找索引节点所在的目录，这个索引节点所对应的文件名就包含在dentry 目录项中
Link(old_dentry, dir,new_dentry)创建一个新的名为new_dentry 硬链接，这个新的硬连接指向dir 目录下名为old_dentry 的文件
unlink(dir, dentry) 从dir 目录删除dentry 目录项所指文件的硬链接
symlink(dir, dentry, symname) 在某个目录下，为与目录项相关的符号链创建一个新的索引节点
mkdir(dir, dentry, mode) 在某个目录下，为与目录项对应的目录创建一个新的索引节点
mknod(dir, dentry, mode,rdev)在dir 目录下，为与目录项对象相关的特殊文件创建一个新的磁盘索引节点。其中参数mode 和 rdev 分别表示文件的类型和该设备的主码
rename(old_dir, old_dentry,new_dir, new_dentry)将old_dir 目录下的文件 old_dentry 移到new_dir 目录下，新文件名包含在 new_dentry 指向的目录项中
readlink(dentry, buffer,buflen)将dentry 所指定的符号链中对应的文件路径名拷贝到buffer 所指定的内存区
follow_link(inode, dir) 解释inode 索引节点所指定的符号链；如果该符号链是相对路径名，从指定的dir 目录开始进行查找
truncate(inode) 修改索引节点inode 所指文件的长度。在调用该方法之前，必须将inode 对象的i_size 域设置为需要的新长度值
permission(inode, mask) 确认是否允许对inode 索引节点所指的文件进行指定模式的访问
revalidate(dentry) 更新由目录项所指定文件的已缓存的属性（通常由网络文件系统调用）
Setattr（dentry，attr） 设置目录项的属性
Getattr（dentry，attr） 获得目录项的属性
```

## 目录操作

![](https://i.loli.net/2018/04/25/5adfda1a6af4b.png)