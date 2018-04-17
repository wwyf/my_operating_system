# 生成kernel.bin
OBJECTS= stdio.o  basic.o  string.o  style.o
C_FLAGS=-c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -Sl

../include/c_run_time.a : $(OBJECTS)
	ar rcs ../include/c_run_time.a $(OBJECTS)
# ALL USER

stdio.o : stdio.c
	gcc $(C_FLAGS) -o stdio.o stdio.c

basic.o: basic.asm
	nasm $(NASM_FLAGS) -o basic.o basic.asm

string.o : string.c
	gcc $(C_FLAGS) -o string.o string.c

style.o : ../lib/style.c
	gcc $(C_FLAGS) -o style.o style.c



.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o