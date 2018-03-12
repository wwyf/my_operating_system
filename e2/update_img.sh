#当变量a为null或为空字符串时则var=b
default_file_name="t"
default_tool_name="q"
file_name=${1:-$default_file_name}  
tool=${2:-$default_tool_name}


nasm -f bin -l $file_name.list $file_name.asm -o $file_name.bin || { echo "nasm complied failed"; exit 1; }
# 如果返回值不为0,则为异常退出,终止脚本
# add the feature : if the image not exist, create the image
if [ ! -f "a.img" ]
then
    echo "a.img not exist!"
    echo "use bximage to create a image!";
    bximage
else
    echo "a.img exists !"
fi
dd if=$file_name.bin of=a.img bs=4096 count=1 conv=notrunc
if [ ! -f "bochsrc.bxrc" ]
then
    echo "bochsrc.bxrc not exist!"
    echo "copy a bochsrc.bxrc to this path quickly";
    exit
else
    echo "bochsrc.bxrc exists !"
fi
# 三个选项
# 可使用bochs 直接打开，可使用qemu打开，也可使用bochsdbg进行调试
# b q d
case $tool in  
 b)  
        bochs -q
        ;;  
 q)  
        qemu-system-i386 -fda a.img
        ;;  
 d)  
        bochsdbg -q  
        ;;  
 *)  
        echo "Usage: $name [b|q|d]"  
        exit 1  
        ;;  
esac
rm *.bin
rm *.list
rm bochsout.txt
rm amsg.txt