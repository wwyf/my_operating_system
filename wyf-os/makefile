objects = a.img
include = include
LOADER = loader
BOOT = boot
KERNEL = kernel


everthing : $(objects) $(BOOT)/boot.bin

$(objects) : $(BOOT)/boot.bin
	dd if=/dev/zero of=a.img ibs=512 count=150 conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=5 conv=notrunc

$(BOOT)/boot.bin:
	cd boot && make
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