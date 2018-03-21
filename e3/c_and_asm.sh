gcc -c -m16 -o c.o my_user_program_t.c
nasm -f elf32 -o asm.o t.asm
ld -o test -Ttext 0x10000 -m elf_i386 -e main c.o asm.o
objcopy -R .note -R .comment -R .eh_frame -S test test.bin