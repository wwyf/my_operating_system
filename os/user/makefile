# 生成boot.bin 引导器，将加载器加载并转交控制权
TEST_BIN = user_test.bin
TEST_ASM = user_e1.asm
include = ../include

all : $(TEST_BIN)

$(TEST_BIN) : $(TEST_ASM) $(include)/macro.inc
	nasm -f bin -o $(TEST_BIN)  $(TEST_ASM)

.PHONY : clean
clean :
	-rm $(TEST_BIN)
	-rm *.list
	-rm *.objdump