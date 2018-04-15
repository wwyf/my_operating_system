# 生成kernel.bin
TARGET = kernel.bin
include = ../include
LIB=../lib
OBJECTS=system_call.o  start.o tty.o fsystem.o global.o style.o
C_FLAGS=-I ../include -c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -Sl
KERNEL_C=start.c
KERNEL_ASM=kernel.asm
LIB_C=$(LIB)/stdio.c
LIB_ASM=$(LIB)/basic.asm

all : $(TARGET) $(OBJECTS) kernel.bin kernel.objdump

start.o: $(KERNEL_C)
	gcc $(C_FLAGS) -o start.o $(KERNEL_C)

kernel.o: $(KERNEL_ASM)
	nasm $(NASM_FLAGS) -o kernel.o $(KERNEL_ASM)

system_call.o : ../lib/system_call.c
	gcc $(C_FLAGS) -o system_call.o ../lib/system_call.c

fsystem.o : ../lib/fsystem.c
	gcc $(C_FLAGS) -o fsystem.o ../lib/fsystem.c

global.o : ../lib/global.c
	gcc $(C_FLAGS) -o global.o ../lib/global.c

style.o : ../lib/style.c
	gcc $(C_FLAGS) -o style.o ../lib/style.c

tty.o : tty.c
	gcc $(C_FLAGS) -o tty.o tty.c

$(TARGET) kernel.debug: $(OBJECTS) kernel.o
	ld $(LINK_FLAGS) -o kernel.bin kernel.o $(OBJECTS) ../include/c_run_time.a
	ld -Ttext 0x00000 -m elf_i386 -T t.lds -o kernel.debug kernel.o $(OBJECTS) ../include/c_run_time.a

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