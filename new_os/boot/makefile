# 生成boot.bin 引导器，将加载器加载并转交控制权
objects = boot
include = ../include
C_FLAGS=-c -m16 -ffreestanding -O0 -g
LINK_FLAGS= -m elf_i386 -T loader.lds --oformat binary
# TODO:连接器已经指定地址为0x08000
DEBUG_LINK_FLAGS= -m elf_i386 -T loader.lds
NASM_FLAGS=-f elf32
OBJDUMP_FLAGS= -m i8086 -Sl

all : boot.bin root.bin fat.bin loader.bin

boot.bin : boot.asm ../include/macro.inc
	nasm -f bin -o boot.bin -l boot.list boot.asm

root.bin : root.asm
	nasm -f bin -o root.bin  root.asm

fat.bin : fat.asm
	nasm -f bin -o fat.bin  fat.asm

loader.bin: loader.c loader_start.asm loader_start.h
	gcc $(C_FLAGS) -o loader.o loader.c
	nasm $(NASM_FLAGS) -o loader_start.o loader_start.asm
	ld $(LINK_FLAGS) -o loader.bin loader.o loader_start.o
	ld $(DEBUG_LINK_FLAGS) -o loader.debug loader.o loader_start.o
	objdump $(OBJDUMP_FLAGS) loader.debug > loader.objdump

.PHONY : clean
clean :
	-rm *.bin
	-rm *.list
	# -rm *.objdump