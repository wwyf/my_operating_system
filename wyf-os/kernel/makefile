INIT_OBJDECT=../init/head.o ../init/main.o
KERNEL_OBJDECT=basic.o protect.o  process.o schedule.o
CHR_DRV_OBJDECT=chr_drv/printk.o chr_drv/debug.o  chr_drv/console.o chr_drv/tty_drv.o
INTR_OBJECT=intr/interrupt.o  intr/interrupt_table.o intr/clock.o
MM_OBJECT=mm/memory.o
TEST_OBJECT=test/test.o
COMMON_OBJECT=common/string.o common/vsprintf.o common/stdlib.o
ALL_OBJDECTS=$(INIT_OBJDECT) $(KERNEL_OBJDECT) $(CHR_DRV_OBJDECT) $(TEST_OBJECT) $(INTR_OBJECT) $(COMMON_OBJECT) $(MM_OBJECT)

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
schedule.o:schedule.c
	gcc schedule.c -o schedule.o $(C_FLAGS)


intr/interrupt.o:intr/interrupt.c
	gcc intr/interrupt.c -o intr/interrupt.o $(C_FLAGS)
intr/interrupt_table.o:intr/interrupt_table.asm
	nasm intr/interrupt_table.asm -o intr/interrupt_table.o $(NASM_FLAGS)
intr/clock.o:intr/clock.c
	gcc intr/clock.c -o intr/clock.o $(C_FLAGS)

	
mm/memory.o:mm/memory.c
	gcc mm/memory.c -o mm/memory.o $(C_FLAGS)

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

common/string.o : common/string.c
	gcc common/string.c -o common/string.o $(C_FLAGS) 

common/vsprintf.o : common/vsprintf.c
	gcc common/vsprintf.c -o common/vsprintf.o $(C_FLAGS) 

common/stdlib.o : common/stdlib.c
	gcc common/stdlib.c -o common/stdlib.o $(C_FLAGS) 



$(INIT_OBJDECT):
	cd ../init && make
	cd ../kernel