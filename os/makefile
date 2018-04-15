# 修改文件位置指南
# 改fat表，改root表的起始簇号，改makefile的dd选项
objects = a.img
include = include
LOADER = loader
BOOT = boot
KERNEL = kernel
USER = user
BIN_FILES = $(BOOT)/boot.bin $(BOOT)/fat.bin $(BOOT)/root.bin $(KERNEL)/kernel.bin $(USER)/test_a.bin $(USER)/test_b.bin
# BIN_FILES = $(BOOT)/boot.bin $(BOOT)/fat.bin $(BOOT)/root.bin $(KERNEL)/kernel.bin $(USER)/user_test.bin  $(USER)/user_e1.bin  $(USER)/user_stone_1.bin $(USER)/user_stone_2.bin $(USER)/user_stone_3.bin $(USER)/user_stone_4.bin
C_RUN_TIME = ../include/c_run_time.a
KERNEL_RUN_TIME = ../include/kernel_run_time.a

everthing : $(objects) $(BIN_FILES)

$(C_RUN_TIME):
	cd lib && make all
	cd ..

$(KERNEL_RUN_TIME):
	cd service && make all
	cd ..

$(BOOT)/boot.bin  $(BOOT)/root.bin $(BOOT)/fat.bin :
	cd boot && make
	cd ..

$(KERNEL)/kernel.bin:$(KERNEL_RUN_TIME)
	cd kernel && make
	cd ..

$(USER)/test_a.bin:$(C_RUN_TIME)
	cd user && make
	cd ..

$(objects) : $(BIN_FILES)
	dd if=/dev/zero of=a.img ibs=512 count=150  conv=notrunc
	dd if=$(BOOT)/boot.bin of=a.img ibs=512 count=1 conv=notrunc
	dd if=$(BOOT)/fat.bin of=a.img ibs=512 count=3 conv=notrunc seek=1
	dd if=$(BOOT)/root.bin of=a.img ibs=512 count=3 conv=notrunc seek=37
	dd if=$(BOOT)/loader.bin of=a.img ibs=512 count=5 conv=notrunc seek=40
	dd if=$(KERNEL)/kernel.bin of=a.img ibs=512 count=18 conv=notrunc seek=46
	dd if=$(USER)/test_a.bin of=a.img ibs=512 count=18 conv=notrunc seek=90
	dd if=$(USER)/test_b.bin of=a.img ibs=512 count=18 conv=notrunc seek=108
	dd if=$(USER)/ouch.bin of=a.img ibs=512 count=18 conv=notrunc seek=126
	# dd if=$(USER)/user_stone_2.bin of=a.img ibs=512 count=5 conv=notrunc seek=144
	# dd if=$(USER)/user_stone_3.bin of=a.img ibs=512 count=5 conv=notrunc seek=162
	# dd if=$(USER)/user_stone_4.bin of=a.img ibs=512 count=5 conv=notrunc seek=180
	# dd if=$(USER)/test_a.bin of=a.img ibs=512 count=5 conv=notrunc seek=198

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
	-rm */*.a
	-rm */*.o
	-rm */*.bin
	-rm */*.list
	# -rm */*.objdump
	-rm ./bochsout.txt