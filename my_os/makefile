objects = a.img
include = include
LOADER = loader
BOOT = boot


everthing : $(objects) $(BOOT)/boot.bin $(LOADER)/loader.bin

$(BOOT)/boot.bin:
	cd boot && make
	cd ..

$(LOADER)/loader.bin:
	cd loader && make
	cd ..

$(objects) : $(BOOT)/boot.bin $(LOADER)/loader.bin $(include)/macro.inc
	dd if=/dev/zero of=a.img ibs=512 count=150 conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=1 conv=notrunc
	dd if=$(LOADER)/loader.bin of=a.img ibs=512 count=18 conv=notrunc seek=36

runq:
	qemu-system-i386.exe -fda a.img
d:
	bochsdbg.exe -q
all:
	make clean
	make everthing
	make runq

.PHONY : clean
clean :
	-rm */*.bin
	-rm */*.list
	-rm */*.objdump