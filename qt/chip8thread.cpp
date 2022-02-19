#include "chip8thread.h"

CHI8Thread::CHI8Thread() {

}

[[noreturn]] void CHI8Thread::run() {
    std::cout << "asdfsaf" << std::endl;
    CHIP8 chip8;
    chip8.init();
    std::string input = "/Users/chunibyo/01_project/06_open_source/19_CHIP8/test_opcode.ch8";
    chip8.loadROM(input);

    Board board;

    while (true) {
        // operation code: 16 bit
//        auto operationCode = chip8.readOperationCode();
//        chip8.processOperationCode(operationCode);

        srand(time(nullptr));
        for (auto &i : board.data)
            std::fill(i, i + 64, 0);
        board.data[rand() % 32][rand() % 64] = true;
        board.data[rand() % 32][rand() % 64] = true;
        emit displaySignal(board);
    }
}
