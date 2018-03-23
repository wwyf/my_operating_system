# 生成mbr.bin 引导器，将内核加载并转交控制权
objects = mbr.bin
include = ../include

all : $(objects)

$(objects) : mbr.asm $(include)/macro.inc
	nasm -f bin -o mbr.bin -l mbr.list mbr.asm

.PHONY : clean
clean :
	-rm $(objects)
	-rm *.list
	-rm *.objdump