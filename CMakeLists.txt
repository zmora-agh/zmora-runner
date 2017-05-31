cmake_minimum_required(VERSION 3.7)
project(zmora_runner)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Werror -Wextra -O3)

add_subdirectory(src)