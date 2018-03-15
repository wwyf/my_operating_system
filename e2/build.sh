nasm.exe -f bin mbr.asm -l mbr.list -o mbr.bin
nasm.exe -f bin user.asm -l user.list -o user.bin
dd if=mbr.bin of=a.img ibs=512 count=1 conv=notrunc
dd if=user.bin of=a.img ibs=512 count=2 conv=notrunc seek=128
# qemu-system-i386.exe -fda a.img
# hexdump -C a.img
bochs.exe -q