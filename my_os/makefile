# 修改文件位置指南
# 改fat表，改root表的起始簇号，改makefile的dd选项
TARGET = a.img
C_RUN_TIME = ../include/c_run_time.a
KERNEL_RUN_TIME = ../include/kernel_run_time.a
BOOT = boot/boot.bin  boot/root.bin boot/fat.bin boot/loader.bin
KERNEL = kernel/kernel.bin
USER = user/test_a.bin  user/test_b.bin user/ouch.bin
BIN_FILES = $(BOOT) $(KERNEL) $(USER)
# BIN_FILES = $(BOOT)/boot.bin $(BOOT)/fat.bin $(BOOT)/root.bin $(KERNEL)/kernel.bin $(USER)/user_test.bin  $(USER)/user_e1.bin  $(USER)/user_stone_1.bin $(USER)/user_stone_2.bin $(USER)/user_stone_3.bin $(USER)/user_stone_4.bin


everything: 
	cd lib && make
	cd ..
	cd service && make
	cd ..
	cd kernel && make all
	cd ..
	cd user && make
	cd ..
	cd boot && make all
	cd ..
	dd if=/dev/zero of=a.img ibs=512 count=300  conv=notrunc
	dd if=boot/boot.bin of=a.img ibs=512 count=1 conv=notrunc
	dd if=boot/fat.bin of=a.img ibs=512 count=3 conv=notrunc seek=1
	dd if=boot/root.bin of=a.img ibs=512 count=3 conv=notrunc seek=37
	dd if=boot/loader.bin of=a.img ibs=512 count=5 conv=notrunc seek=40
	dd if=kernel/kernel.bin of=a.img ibs=512 count=40 conv=notrunc seek=46
	dd if=user/test_a.bin of=a.img ibs=512 count=18 conv=notrunc seek=90
	dd if=user/test_b.bin of=a.img ibs=512 count=18 conv=notrunc seek=108
	dd if=user/ouch.bin of=a.img ibs=512 count=18 conv=notrunc seek=126
	# dd if=$(USER)/user_stone_2.bin of=a.img ibs=512 count=5 conv=notrunc seek=144
	# dd if=$(USER)/user_stone_3.bin of=a.img ibs=512 count=5 conv=notrunc seek=162
	# dd if=$(USER)/user_stone_4.bin of=a.img ibs=512 count=5 conv=notrunc seek=180
	# dd if=$(USER)/test_a.bin of=a.img ibs=512 count=5 conv=notrunc seek=198

runq:
	qemu-system-i386.exe -fda a.img
d:
	bochsdbg.exe -q
all:
	# make clean
	make everything
	make runq
	# make clean

.PHONY : clean
clean :
	# -rm */*.a
	-rm */*.o
	-rm */*.bin
	-rm */*.list
	# -rm */*.objdump
	-rm ./bochsout.txt