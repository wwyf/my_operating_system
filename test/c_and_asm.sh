nasm -f elf32 foo.asm -o asm.o
gcc -c -m16 -o c.o bar.c
ld -s -m elf_i8086 -o test.out c.o asm.o