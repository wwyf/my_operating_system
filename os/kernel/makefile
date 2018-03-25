# 生成kernel.bin
TARGET = kernel.bin
include = ../include
LIB=../lib
OBJECTS=stdio.o  basic.o  kernel.o string.o interrupt.o keyboard.o
C_FLAGS=-c -m16 -ffreestanding -O0
NASM_FLAGS=-f elf32
# LINK_FLAGS=-Ttext 0x10000 -m elf_i386 -T t.lds --oformat binary
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -b binary
KERNEL_C=kernel.c
LIB_C=$(LIB)/stdio.c
LIB_ASM=$(LIB)/basic.asm

all : $(TARGET) $(OBJECTS)

stdio.o : $(LIB_C)
	gcc $(C_FLAGS) -o stdio.o $(LIB_C)

basic.o: $(LIB_ASM)
	nasm $(NASM_FLAGS) -o basic.o $(LIB_ASM) 

kernel.o: $(KERNEL_C)
	gcc $(C_FLAGS) -o kernel.o $(KERNEL_C)

string.o : ../lib/string.c
	gcc $(C_FLAGS) -o string.o ../lib/string.c

interrupt.o : ../lib/interrupt.c
	nasm $(NASM_FLAGS) -o interrupt.o ../lib/interrupt.asm

keyboard.o : ../lib/keyboard.c
	gcc $(C_FLAGS) -o keyboard.o ../lib/keyboard.c

$(TARGET): $(OBJECTS)
	ld $(LINK_FLAGS) -o kernel.bin $(OBJECTS)
	objdump $(OBJDUMP_FLAGS) -D kernel.bin > kernel.objdump




.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o