objects = a.img
include = include
LOADER = loader
BOOT = boot
KERNEL = kernel


everthing : $(objects) $(BOOT)/boot.bin $(LOADER)/loader.bin $(KERNEL)/kernel.bin

$(BOOT)/boot.bin:
	cd boot && make
	cd ..

$(LOADER)/loader.bin:
	cd loader && make
	cd ..

$(KERNEL)/kernel.bin:
	cd kernel && make
	cd ..

$(objects) : $(BOOT)/boot.bin $(LOADER)/loader.bin $(KERNEL)/kernel.bin $(include)/macro.inc
	dd if=/dev/zero of=a.img ibs=512 count=150 conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=1 conv=notrunc
	dd if=$(LOADER)/loader.bin of=a.img ibs=512 count=18 conv=notrunc seek=36
	dd if=$(KERNEL)/kernel.bin of=a.img ibs=512 count=18 conv=notrunc seek=54

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