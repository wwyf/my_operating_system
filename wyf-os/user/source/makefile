# 生成kernel.bin
TARGET=libcrt.a
OBJECTS= string.o vsprintf.o stdlib.o
C_FLAGS=-c -I ./include -m32 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
OBJDUMP_FLAGS= -m i386 -Sl

../lib/libcrt.a : $(OBJECTS)
	ar rcs ../lib/libcrt.a $(OBJECTS)

# ALL USER
string.o : string.c
	gcc string.c -o string.o $(C_FLAGS) 

vsprintf.o : vsprintf.c
	gcc vsprintf.c -o vsprintf.o $(C_FLAGS) 

stdlib.o : stdlib.c
	gcc stdlib.c -o stdlib.o $(C_FLAGS) 

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o