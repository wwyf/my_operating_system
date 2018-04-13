# 生成boot.bin 引导器，将加载器加载并转交控制权
TEST_BIN = user_test.bin
TEST_ASM = user_test.asm
include = ../include

USER_BIN = user_test.bin user_e1.bin user_stone_1.bin

all : $(USER_BIN)

user_test.bin : $(TEST_ASM) $(include)/macro.inc
	nasm -f bin -o $(TEST_BIN)  $(TEST_ASM)

user_e1.bin : user_e1.asm $(include)/macro.inc
	nasm -f bin -o user_e1.bin user_e1.asm

user_stone_1.bin : user_stone_1.asm $(include)/macro.inc
	nasm -f bin -o user_stone_1.bin user_stone_1.asm

.PHONY : clean
clean :
	-rm $(TEST_BIN)
	-rm *.list
	-rm *.objdump