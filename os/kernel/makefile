# 生成kernel.bin
TARGET = kernel.bin
include = ../include
LIB=../lib
OBJECTS=start.o
C_FLAGS=-I ../include -c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
DEBUG_LINK_FLAGS=  -Ttext 0x00000 -m elf_i386 -T t.lds
OBJDUMP_FLAGS= -m i8086 -Sl

all: kernel.bin kernel.objdump

kernel.bin kernel.objdump:kernel.o $(OBJECTS) ../include/kernel_run_time.a  kernel.debug
	ld $(LINK_FLAGS) -o kernel.bin kernel.o $(OBJECTS) ../include/kernel_run_time.a
	objdump $(OBJDUMP_FLAGS) kernel.debug > kernel.objdump

kernel.debug:kernel.o $(OBJECTS)  ../include/kernel_run_time.a
	ld $(DEBUG_LINK_FLAGS) -o kernel.debug kernel.o $(OBJECTS) ../include/kernel_run_time.a

kernel.o: kernel.asm
	nasm $(NASM_FLAGS) -o kernel.o kernel.asm

start.o: start.c
	gcc $(C_FLAGS) -o start.o start.c

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o