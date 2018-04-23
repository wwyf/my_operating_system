# 生成boot.bin 引导器，将加载器加载并转交控制权
objects = boot
include = ../include

all : $(objects).bin

$(objects).bin : $(objects).asm $(include)/macro.inc
	nasm -f bin -o $(objects).bin -l $(objects).list $(objects).asm

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump