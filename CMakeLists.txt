# cmake_minimum_required(VERSION <specify CMake version here>)
project(19_CHIP8)

set(CMAKE_CXX_STANDARD 17)

add_executable(19_CHIP8 main.cpp CHIP8.cpp CHIP8.h)