C_FLAGS=-I ../include -c -m32 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-e kernel_head_start -Ttext 0x00000 -m elf_i386 --oformat binary
LINK_FLAGS_DEBUG=-e kernel_head_start -Ttext 0x00000 -m elf_i386
DEBUG_LINK_FLAGS=  -Ttext 0x00000 -m elf_i386 -T t.lds
OBJDUMP_FLAGS= -m i386 -Sl

all:head.o main.o

head.o:head.asm
	nasm head.asm -o head.o $(NASM_FLAGS)
main.o:main.c
	gcc main.c -o main.o $(C_FLAGS)

clean:
	-rm *.bin
	-rm *.o
	-rm *.elf
	-rm *.debug