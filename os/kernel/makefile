# 生成kernel.bin
TARGET = kernel.bin
include = ../include
LIB=../lib
OBJECTS=start.o tty.o
C_FLAGS=-I ../include -c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -Sl

all : $(TARGET) $(OBJECTS) kernel.bin kernel.objdump

kernel.o: kernel.asm
	nasm $(NASM_FLAGS) -o kernel.o kernel.asm

start.o: start.c
	gcc $(C_FLAGS) -o start.o start.c

tty.o : tty.c
	gcc $(C_FLAGS) -o tty.o tty.c

$(TARGET) kernel.debug: $(OBJECTS) kernel.o
	ld $(LINK_FLAGS) -o kernel.bin kernel.o $(OBJECTS) ../include/kernel_run_time.a
	ld -Ttext 0x00000 -m elf_i386 -T t.lds -o kernel.debug kernel.o $(OBJECTS) ../include/kernel_run_time.a

kernel.objdump:kernel.debug
	objdump $(OBJDUMP_FLAGS) kernel.debug > kernel.dump




.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o
# user_ouch.bin: user_ouch.c
# 	gcc $(C_FLAGS) -o user_ouch.o user_ouch.c
# 	ld -Ttext 0x00 -m elf_i386 --oformat binary -o user_ouch.bin user_ouch.o basic.o stdio.o string.o