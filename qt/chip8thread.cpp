#include "chip8thread.h"

CHI8Thread::CHI8Thread() {

}

void CHI8Thread::run() {
    std::cout << "asdfsaf" << std::endl;
    CHIP8 chip8;
    chip8.init();
    std::string input = "/Users/chunibyo/01_project/06_open_source/19_CHIP8/test_opcode.ch8";
    chip8.loadROM(input);
    chip8.run();
}
