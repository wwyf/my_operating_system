# 生成kernel.bin
TARGET = kernel.bin
include = ../include
LIB=../lib
OBJECTS= stdio.o  basic.o  string.o system_call.o  start.o tty.o fsystem.o
C_FLAGS=-c -m16 -ffreestanding -Og
NASM_FLAGS=-f elf32
# LINK_FLAGS=-Ttext 0x10000 -m elf_i386 -T t.lds --oformat binary
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -b binary
KERNEL_C=start.c
KERNEL_ASM=kernel.asm
LIB_C=$(LIB)/stdio.c
LIB_ASM=$(LIB)/basic.asm

all : $(TARGET) $(OBJECTS) kernel.bin

stdio.o : $(LIB_C)
	gcc $(C_FLAGS) -o stdio.o $(LIB_C)

basic.o: $(LIB_ASM)
	nasm $(NASM_FLAGS) -o basic.o $(LIB_ASM) 

start.o: $(KERNEL_C)
	gcc $(C_FLAGS) -o start.o $(KERNEL_C)

kernel.o: $(KERNEL_ASM)
	nasm $(NASM_FLAGS) -o kernel.o $(KERNEL_ASM)

string.o : ../lib/string.c
	gcc $(C_FLAGS) -o string.o ../lib/string.c

system_call.o : ../lib/system_call.c
	gcc $(C_FLAGS) -o system_call.o ../lib/system_call.c

fsystem.o : ../lib/fsystem.c
	gcc $(C_FLAGS) -o fsystem.o ../lib/fsystem.c


# keyboard.o : ../lib/keyboard.c
	# gcc $(C_FLAGS) -o keyboard.o ../lib/keyboard.

tty.o : tty.c
	gcc $(C_FLAGS) -o tty.o tty.c

$(TARGET): $(OBJECTS) kernel.o
	ld $(LINK_FLAGS) -o kernel.bin kernel.o $(OBJECTS)
	objdump $(OBJDUMP_FLAGS) -D kernel.bin > kernel.objdump




.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o