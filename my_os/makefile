objects = a.img
include = include
loader = loader

all : $(objects)

$(objects) : $(loader)/mbr.bin  $(include)/macro.inc
	dd if=/dev/zero of=a.img ibs=512 count=150 conv=notrunc
	dd if=$(loader)/mbr.bin of=a.img ibs=512 count=1 conv=notrunc

$(loader)/mbr.bin :
	cd loader && make
	cd ..

bar:
	

runq:
	qemu-system-i386.exe -fda a.img
debug:
	bochsdbg.exe -q

.PHONY : clean
clean :
	-rm */*.bin
	-rm */*.list
	-rm */*.objdump