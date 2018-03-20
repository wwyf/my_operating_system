nasm.exe -f bin my_mbr.asm -l my_mbr.list -o my_mbr.bin    || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_core.asm -l my_core.list -o my_core.bin   || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_user_program_1.asm -l my_user_program_1.list -o my_user_program_1.bin   || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_user_program_2.asm -l my_user_program_2.list -o my_user_program_2.bin   || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_user_program_3.asm -l my_user_program_3.list -o my_user_program_3.bin   || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_user_program_4.asm -l my_user_program_4.list -o my_user_program_4.bin   || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_user_program_l.asm -l my_user_program_l.list -o my_user_program_l.bin   || { echo "nasm complied failed"; exit 1; }
nasm.exe -f bin my_user_program_t.asm -l my_user_program_t.list -o my_user_program_t.bin   || { echo "nasm complied failed"; exit 1; }
# test for complie with C
# nasm -f elf32 my_user_program_t.asm -o my_user_program_t_asm.o || { echo "nasm complied failed"; exit 1; }
# gcc -m32 -c  my_user_program_t.c  -o my_user_program_t_c.o || { echo "gcc complied failed"; exit 1; }
# ld -s -m elf_i386 -o my_user_program_t.bin  my_user_program_t_asm.o my_user_program_t_c.o || { echo "link failed"; exit 1; }
dd if=/dev/zero of=a.img ibs=512 count=100 conv=notrunc
dd if=my_mbr.bin of=a.img ibs=512 count=1 conv=notrunc
dd if=my_core.bin of=a.img ibs=512 count=10 conv=notrunc seek=1
dd if=my_user_program_1.bin of=a.img ibs=512 count=10 conv=notrunc seek=18
dd if=my_user_program_2.bin of=a.img ibs=512 count=10 conv=notrunc seek=36
dd if=my_user_program_3.bin of=a.img ibs=512 count=10 conv=notrunc seek=54
dd if=my_user_program_4.bin of=a.img ibs=512 count=10 conv=notrunc seek=72
dd if=my_user_program_l.bin of=a.img ibs=512 count=10 conv=notrunc seek=90
dd if=my_user_program_t.bin of=a.img ibs=512 count=10 conv=notrunc seek=108
# qemu-system-i386.exe -fda a.img
# hexdump -C a.img
# bochs.exe -q
# bochsdbg.exe -q

case $1 in
b)
        bochs.exe -q
        ;;  
q)
        qemu-system-i386.exe -fda a.img
        ;;  
d)
        bochsdbg.exe -q  
        ;;  
*)
        echo "Usage: $name [b|q|d]"  
        exit 1  
        ;;  
esac

rm *.bin
rm *.list
rm bochsout.txt