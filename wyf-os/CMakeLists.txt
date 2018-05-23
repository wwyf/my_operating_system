cmake_minimum_required(VERSION 3.5)
project(wyf_os)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_custom_target(wyf_os COMMAND make all -C ${wyf_os_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})