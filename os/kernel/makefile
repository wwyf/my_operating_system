# 生成kernel.bin
objects = kernel.bin
include = ../include
LIB=../lib

C_FLAGS=-c -m16 -ffreestanding
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x10000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -b binary 
KERNEL_C=kernel.c
LIB_C=$(LIB)/stdio.c
LIB_ASM=$(LIB)/basic.asm

all : $(objects)

$(objects):
	gcc $(C_FLAGS) -o kernel.o $(KERNEL_C)
	gcc $(C_FLAGS) -o stdio.o $(LIB_C)
	nasm $(NASM_FLAGS) -o lib.o $(LIB_ASM) 
	ld $(LINK_FLAGS) -o kernel.bin  kernel.o  lib.o  stdio.o
	objdump $(OBJDUMP_FLAGS) -D kernel.bin > kernel.objdump
# $(objects).bin : $(objects).asm $(include)/macro.inc
	# nasm -f bin -o $(objects).bin -l $(objects).list $(objects).asm

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o