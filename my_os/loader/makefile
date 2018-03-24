# 生成loader.bin 加载器，将系统内核加载并转交控制权
objects = loader.bin
include = ../include

all : $(objects)  gdt.bin

$(objects) : loader.asm $(include)/macro.inc
	nasm -f bin -o $(objects) -l loader.list loader.asm

gdt.bin : gdt.asm $(include)/pm.inc
	nasm -f bin -o gdt.bin -l gdt.list gdt.asm


.PHONY : clean
clean :
	-rm $(objects)
	-rm *.list
	-rm *.objdump