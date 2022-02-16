//
// Created by chunibyo on 2022/2/16.
//

#ifndef INC_19_CHIP8_CHIP8_H
#define INC_19_CHIP8_CHIP8_H

#include <iostream>
#include <fstream>

typedef unsigned char Byte;
typedef unsigned short Word;

struct Memory {
    // PC 的起始位置
    static constexpr Word pcStartMemory = 0x200;

    // 内存的最大值
    static constexpr Word endMemory = 0xFFF;

    Byte &operator[](Word &address);

    // 4KB 大小内存
    // https://en.wikipedia.org/wiki/CHIP-8#Memory
    Byte memory[endMemory + 1]{};

    bool loadROM(std::string &file);

    void clear();
};

struct CPU {
    Memory memory;

    // 16个8Bit(1Byte)个大小的寄存器
    // https://en.wikipedia.org/wiki/CHIP-8#Registers
    Byte VRegister[16]{};

    // 程序计数器: 16 Bit
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.2
    Word PC{};

    // stack pointer: 8 Bit
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.2
    Byte SP{};

    void pcUpdate();
};

struct CHIP8 {
    CPU cpu;

    void init();

    void reset();

    bool loadROM(std::string &file);

    [[noreturn]] void run();

    Word readOperationCode();
};


#endif //INC_19_CHIP8_CHIP8_H
