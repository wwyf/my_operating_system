# 生成kernel.bin
OBJECTS= string.o vsprintf.o
C_FLAGS=-c -I ../include -m32 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
OBJDUMP_FLAGS= -m i386 -Sl

../include/libmycrt.a : $(OBJECTS)
	ar rcs ../include/libmycrt.a $(OBJECTS)

# ALL USER
string.o : string.c
	gcc string.c -o string.o $(C_FLAGS) 

vsprintf.o : vsprintf.c
	gcc vsprintf.c -o vsprintf.o $(C_FLAGS) 

.PHONY : clean
clean :
	-rm $(objects).bin
	-rm *.list
	-rm *.objdump
	-rm *.o