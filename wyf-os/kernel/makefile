INIT_OBJDECT=../init/head.o ../init/main.o
KERNEL_OBJDECT=test.o vsprintf.o

ALL_OBJDECTS=$(INIT_OBJDECT) $(KERNEL_OBJDECT)

C_FLAGS=-I ../include -c -m32 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-e kernel_head_start -Ttext 0x00000 -m elf_i386 --oformat binary
LINK_FLAGS_DEBUG=-e kernel_head_start -Ttext 0x00000 -m elf_i386
DEBUG_LINK_FLAGS=  -Ttext 0x00000 -m elf_i386 -T t.lds
OBJDUMP_FLAGS= -m i386 -Sl

all:kernel.bin kernel.elf

kernel.bin:$(ALL_OBJDECTS)
	ld $(ALL_OBJDECTS) -o ../kernel.bin $(LINK_FLAGS)
kernel.elf:$(ALL_OBJDECTS)
	ld $(ALL_OBJDECTS) -o ../kernel.elf $(LINK_FLAGS_DEBUG)
	objdump ../kernel.elf  $(OBJDUMP_FLAGS) > ../kernel.objdump

vsprintf.o:vsprintf.c
	gcc vsprintf.c -o vsprintf.o $(C_FLAGS)
test.o:test.c
	gcc test.c -o test.o $(C_FLAGS)

$(INIT_OBJDECT):
	cd ../init && make
	cd ../kernel