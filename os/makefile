objects = a.img
include = include
LOADER = loader
BOOT = boot
KERNEL = kernel
USER = user

everthing : $(objects) $(BOOT)/boot.bin $(BOOT)/fat.bin $(BOOT)/root.bin $(KERNEL)/kernel.bin $(USER)/user_test.bin

$(BOOT)/boot.bin  $(BOOT)/root.bin $(BOOT)/fat.bin :
	cd boot && make
	cd ..

$(KERNEL)/kernel.bin:
	cd kernel && make
	cd ..

$(USER)/user_test.bin:
	cd user && make
	cd ..

$(objects) : $(BOOT)/boot.bin $(BOOT)/root.bin $(BOOT)/fat.bin $(KERNEL)/kernel.bin $(USER)/user_test.bin $(include)/macro.inc
	dd if=/dev/zero of=a.img ibs=512 count=150  conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=1 conv=notrunc
	dd if=$(BOOT)/fat.bin of=a.img ibs=512 count=1 conv=notrunc seek=1
	dd if=$(BOOT)/root.bin of=a.img ibs=512 count=1 conv=notrunc seek=37
	# dd if=$(LOADER)/gdt.bin of=a.img ibs=512 count=18 conv=notrunc seek=35
	# dd if=$(LOADER)/loader.bin of=a.img ibs=512 count=18 conv=notrunc seek=36
	dd if=$(KERNEL)/kernel.bin of=a.img ibs=512 count=18 conv=notrunc seek=72
	dd if=$(USER)/user_test.bin of=a.img ibs=512 count=18 conv=notrunc seek=90

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
	-rm */*.o
	-rm */*.bin
	-rm */*.list
	-rm */*.objdump
	-rm ./bochsout.txt