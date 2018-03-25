# 生成kernel.bin
objects = kernel.bin
include = ../include
LIB=../lib

C_FLAGS=-c -m16 -ffreestanding -O
NASM_FLAGS=-f elf32
# LINK_FLAGS=-Ttext 0x10000 -m elf_i386 -T t.lds --oformat binary
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -b binary
KERNEL_C=kernel.c
LIB_C=$(LIB)/stdio.c
LIB_ASM=$(LIB)/basic.asm

all : $(objects) stdio.o basic.o  kernel.o

stdio.o : $(LIB_C)
	gcc $(C_FLAGS) -o stdio.o $(LIB_C)

basic.o: $(LIB_ASM)
	nasm $(NASM_FLAGS) -o basic.o $(LIB_ASM) 

kernel.o: $(KERNEL_C)
	gcc $(C_FLAGS) -o kernel.o $(KERNEL_C)

$(objects): stdio.o  basic.o  kernel.o
	ld $(LINK_FLAGS) -o kernel.bin  kernel.o  basic.o  stdio.o
	objdump $(OBJDUMP_FLAGS) -D kernel.bin > kernel.objdump




.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o