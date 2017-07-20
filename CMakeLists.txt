cmake_minimum_required(VERSION 3.7)
project(zmora_runner)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Werror -Wextra -O3)
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static")

if (NOT DEFINED VERSION)
    string(TIMESTAMP VERSION)
endif ()

add_subdirectory(src)