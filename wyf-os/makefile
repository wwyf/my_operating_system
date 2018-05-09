objects = a.img
include = include
LOADER = loader
BOOT = boot
KERNEL = kernel
INIT = init

everthing : $(objects) $(BOOT)/boot.bin $(INIT)/kernel.bin

$(objects) : $(BOOT)/boot.bin $(INIT)/kernel.bin
	dd if=/dev/zero of=a.img ibs=512 count=150 conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=5 conv=notrunc
	dd if=$(INIT)/kernel.bin of=a.img ibs=512 count=10 conv=notrunc seek=5

$(BOOT)/boot.bin:
	cd boot && make
	cd ..

$(INIT)/kernel.bin:
	cd init && make
	cd ..

runq:
	qemu-system-i386.exe -fda a.img
d:
	bochsdbg.exe -q
all:
	make clean
	make everthing
	make runq
	# make clean

.PHONY : clean
clean :
	-rm */*.bin
	-rm */*.list
	-rm */*.objdump
	-rm */*.elf
	-rm */*.o