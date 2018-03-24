# 生成loader.bin 加载器，将系统内核加载并转交控制权
objects = kernel
include = ../include

all : $(objects).bin

$(objects).bin : $(objects).asm $(include)/macro.inc
	nasm -f bin -o $(objects).bin -l $(objects).list $(objects).asm

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump