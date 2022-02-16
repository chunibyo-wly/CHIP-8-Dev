#include <iostream>
#include "CHIP8.h"

int main() {
    CHIP8 chip8;
    chip8.init();
    std::string input = "test_opcode.ch8";
    chip8.loadROM(input);
    chip8.run();
}
