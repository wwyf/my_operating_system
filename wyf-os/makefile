objects=a.img
include=include
LOADER=loader
BOOT=boot
KERNEL=kernel
INIT=init

everthing : $(objects) $(BOOT)/boot.bin kernel.bin

$(objects) : $(BOOT)/boot.bin kernel.bin
	dd if=/dev/zero of=a.img ibs=512 count=150 conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=5 conv=notrunc
	dd if=kernel.bin of=a.img ibs=512 count=300 conv=notrunc seek=5

$(BOOT)/boot.bin:
	cd boot && make
	cd ..

kernel.bin:$(INIT)/main.o $(INIT)/head.o
	cd kernel && make
	cd ..

$(INIT)/main.o $(INIT)/head.o:
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
	-rm bochsout.txt
	-rm *.bin
	-rm *.elf
	-rm */*.bin
	-rm */*.list
	-rm */*.objdump
	-rm */*.elf
	-rm */*.o
	-rm */*/*.o