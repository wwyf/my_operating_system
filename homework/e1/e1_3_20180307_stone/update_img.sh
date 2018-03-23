name=$1
nasm -f bin -l $1.list $1.asm -o $1.bin || { echo "nasm complied failed"; exit 1; }
# 如果返回值不为0,则为异常退出,终止脚本
# add the feature : if the image not exist, create the image
if [ ! -f "a.img" ]
then
    echo "a.img not exist!"
    echo "use bximage to create a image";
    exit
else
    echo "a.img exists !"
fi
dd if=$1.bin of=a.img bs=4096 count=1 conv=notrunc
if [ ! -f "bochsrc.bxrc" ]
then
    echo "bochsrc.bxrc not exist!"
    echo "copy a bochsrc.bxrc to this path quickly";
    exit
else
    echo "bochsrc.bxrc exists !"
fi
# bochs -q
qemu-system-i386 -fda a.img