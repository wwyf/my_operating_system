
USER_BIN = test_a.bin test_b.bin
C_FLAGS=-I ../include -c -m16 -ffreestanding -Og -g
NASM_FLAGS=-f elf32
LINK_FLAGS=-Ttext 0x00000 -m elf_i386 -T t.lds --oformat binary
OBJDUMP_FLAGS= -m i8086 -Sl

# USER_BIN = user_test.bin user_e1.bin user_stone_1.bin user_stone_2.bin user_stone_3.bin user_stone_4.bin

all : $(USER_BIN)


test_a.bin : test_a.c
	gcc $(C_FLAGS) -o test_a.o test_a.c
	ld $(LINK_FLAGS) -o test_a.bin test_a.o ../include/c_run_time.a

test_b.bin : test_b.c
	gcc $(C_FLAGS) -o test_b.o test_b.c
	ld $(LINK_FLAGS) -o test_b.bin test_b.o ../include/c_run_time.a

# user_test.bin : $(TEST_ASM) $(include)/macro.inc
# 	nasm -f bin -o $(TEST_BIN)  $(TEST_ASM)

# user_e1.bin : user_e1.asm $(include)/macro.inc
# 	nasm -f bin -o user_e1.bin user_e1.asm

# user_stone_1.bin : user_stone_1.asm $(include)/macro.inc
# 	nasm -f bin -o user_stone_1.bin user_stone_1.asm

# user_stone_2.bin : user_stone_2.asm $(include)/macro.inc
# 	nasm -f bin -o user_stone_2.bin user_stone_2.asm

# user_stone_3.bin : user_stone_3.asm $(include)/macro.inc
# 	nasm -f bin -o user_stone_3.bin user_stone_3.asm

# user_stone_4.bin : user_stone_4.asm $(include)/macro.inc
# 	nasm -f bin -o user_stone_4.bin user_stone_4.asm

.PHONY : clean
clean :
	-rm $(TEST_BIN)
	-rm *.list
	-rm *.objdump