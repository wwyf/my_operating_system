INIT_OBJDECT=../init/head.o ../init/main.o
KERNEL_OBJDECT=test.o vsprintf.o basic.o 
CHR_DRV_OBJDECT=chr_drv/console.o chr_drv/tty_drv.o
ALL_OBJDECTS=$(INIT_OBJDECT) $(KERNEL_OBJDECT) $(CHR_DRV_OBJDECT)

C_FLAGS=-I ../include -c -m32 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-e kernel_head_start -L../include -Ttext 0x00000 -m elf_i386 --oformat binary -lmycrt
LINK_FLAGS_DEBUG=-e kernel_head_start -L../include -Ttext 0x00000 -m elf_i386 -lmycrt
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
basic.o:basic.asm
	nasm basic.asm -o basic.o $(NASM_FLAGS)

chr_drv/console.o:chr_drv/console.c
	gcc chr_drv/console.c -o chr_drv/console.o $(C_FLAGS)
chr_drv/tty_drv.o:chr_drv/tty_drv.c
	gcc chr_drv/tty_drv.c -o chr_drv/tty_drv.o $(C_FLAGS)



$(INIT_OBJDECT):
	cd ../init && make
	cd ../kernel