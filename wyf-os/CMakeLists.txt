cmake_minimum_required(VERSION 3.5)
project(wyf_os)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(wyf_os COMMAND make all -C ${wyf_os_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})

include_directories(include)
include_directories(include/chr_drv)
include_directories(include/common)
include_directories(include/intr)
include_directories(include/mm)
include_directories(include/proc)
include_directories(include/protect)
include_directories(include/test)

add_executable(useless
        include/chr_drv/console.h
        include/chr_drv/tty_drv.h
        include/common/common.h
        include/common/debug.h
        include/common/stdarg.h
        include/common/stdlib.h
        include/common/string.h
        include/intr/clock.h
        include/intr/interrupt.h
        include/mm/memory.h
        include/mm/page.h
        include/proc/process.h
        include/proc/schedule.h
        include/protect/protect.h
        include/test/test.h
        include/basic.h
        include/const.h
        include/global.h
        include/type.h
        init/main.c
        kernel/chr_drv/console.c
        kernel/chr_drv/debug.c
        kernel/chr_drv/printk.c
        kernel/chr_drv/tty_drv.c
        kernel/common/stdlib.c
        kernel/common/string.c
        kernel/common/vsprintf.c
        kernel/intr/clock.c
        kernel/intr/interrupt.c
        kernel/mm/memory.c
        kernel/mm/page.c
        kernel/test/test.c
        kernel/process.c
        kernel/protect.c
        kernel/schedule.c)
