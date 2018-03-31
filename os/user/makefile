# 生成boot.bin 引导器，将加载器加载并转交控制权
objects = user_test.bin
include = ../include

all : $(objects)

$(objects) : user_test.asm $(include)/macro.inc
	nasm -f bin -o user_test.bin  user_test.asm

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump