INIT_OBJDECT=../init/head.o ../init/main.o
KERNEL_OBJDECT=basic.o protect.o interrupt.o interrupt_table.o process.o
CHR_DRV_OBJDECT=chr_drv/printk.o chr_drv/debug.o  chr_drv/console.o chr_drv/tty_drv.o
TEST_OBJECT=test/test.o
ALL_OBJDECTS=$(INIT_OBJDECT) $(KERNEL_OBJDECT) $(CHR_DRV_OBJDECT) $(TEST_OBJECT)

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

basic.o:basic.asm
	nasm basic.asm -o basic.o $(NASM_FLAGS)
protect.o:protect.c
	gcc protect.c -o protect.o $(C_FLAGS)
process.o:process.c
	gcc process.c -o process.o $(C_FLAGS)
interrupt.o:interrupt.c
	gcc interrupt.c -o interrupt.o $(C_FLAGS)
interrupt_table.o:interrupt_table.asm
	nasm interrupt_table.asm -o interrupt_table.o $(NASM_FLAGS)


chr_drv/debug.o:chr_drv/debug.c
	gcc chr_drv/debug.c -o chr_drv/debug.o $(C_FLAGS)
chr_drv/printk.o:chr_drv/printk.c
	gcc chr_drv/printk.c -o chr_drv/printk.o $(C_FLAGS)
chr_drv/console.o:chr_drv/console.c
	gcc chr_drv/console.c -o chr_drv/console.o $(C_FLAGS)
chr_drv/tty_drv.o:chr_drv/tty_drv.c
	gcc chr_drv/tty_drv.c -o chr_drv/tty_drv.o $(C_FLAGS)
chr_drv/tty.o:chr_drv/tty.c
	gcc chr_drv/tty.c -o chr_drv/tty.o $(C_FLAGS)


test/test.o:test/test.c
	gcc test/test.c -o test/test.o $(C_FLAGS)


$(INIT_OBJDECT):
	cd ../init && make
	cd ../kernel