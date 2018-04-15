# 生成kernel.bin
OBJECTS= stdio.o  basic.o  string.o  style.o
KERNEL_OBJECTS=system_call.o fsystem.o global.o 
C_FLAGS=-c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -Sl

all : $(OBJECTS) $(KERNEL_OBJECTS)
	ar rcs ../include/c_run_time.a $(OBJECTS)
	ar rcs ../include/kernel_run_time.a $(KERNEL_OBJECTS) $(OBJECTS)
	-rm *.o
# ALL USER

stdio.o : stdio.c
	gcc $(C_FLAGS) -o stdio.o stdio.c

basic.o: basic.asm
	nasm $(NASM_FLAGS) -o basic.o basic.asm

string.o : string.c
	gcc $(C_FLAGS) -o string.o string.c

style.o : ../lib/style.c
	gcc $(C_FLAGS) -o style.o ../lib/style.c

# KERNEL ONLY 

system_call.o : ../lib/system_call.c
	gcc $(C_FLAGS) -o system_call.o ../lib/system_call.c

fsystem.o : ../lib/fsystem.c
	gcc $(C_FLAGS) -o fsystem.o ../lib/fsystem.c

global.o : ../lib/global.c
	gcc $(C_FLAGS) -o global.o ../lib/global.c


.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o