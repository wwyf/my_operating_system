# 生成kernel.bin
OBJECTS= stdio.o  basic.o  string.o  style.o
KERNEL_OBJECTS=system_call.o fsystem.o global.o proc.o tty.o
C_FLAGS=-c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -Sl

../include/kernel_run_time.a : $(OBJECTS) $(KERNEL_OBJECTS)
	ar rcs ../include/kernel_run_time.a $(KERNEL_OBJECTS) $(OBJECTS)
# ALL USER

stdio.o : ../lib/stdio.c
	gcc $(C_FLAGS) -o stdio.o ../lib/stdio.c

basic.o: ../lib/basic.asm
	nasm $(NASM_FLAGS) -o basic.o ../lib/basic.asm

string.o : ../lib/string.c
	gcc $(C_FLAGS) -o string.o ../lib/string.c

style.o : ../lib/style.c
	gcc $(C_FLAGS) -o style.o ../lib/style.c

# KERNEL ONLY 

system_call.o : system_call.c
	gcc $(C_FLAGS) -o system_call.o system_call.c

fsystem.o : fsystem.c
	gcc $(C_FLAGS) -o fsystem.o fsystem.c

global.o : global.c
	gcc $(C_FLAGS) -o global.o global.c

proc.o : proc.c
	gcc $(C_FLAGS) -o proc.o proc.c


tty.o : tty.c
	gcc $(C_FLAGS) -o tty.o tty.c


.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o