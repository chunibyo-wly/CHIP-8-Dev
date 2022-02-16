//
// Created by chunibyo on 2022/2/16.
//

#include "CHIP8.h"

void CHIP8::init() {
    // Most Chip-8 programs start at location 0x200 (512)
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1
    cpu.PC = Memory::pcStartMemory;

    cpu.memory.clear();
}

void CHIP8::reset() {

}

bool CHIP8::loadROM(std::string &file) {
    // TODO: check file exist
    return cpu.memory.loadROM(file);
}

[[noreturn]] void CHIP8::run() {
    while (true) {
        // operation code: 16 bit
        auto operationCode = readOperationCode();
    }
}

Word CHIP8::readOperationCode() {
    // All instructions are 2 bytes long
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.0
    Word tmp = cpu.memory[cpu.PC] << 8;
    cpu.pcUpdate();
    tmp |= cpu.memory[cpu.PC];
    cpu.pcUpdate();
    return tmp;
}

bool Memory::loadROM(std::string &file) {
    // step 1: 计算文件大小
    std::ifstream infile(file);
    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    if (length + pcStartMemory > endMemory) {
        std::cerr << "ROM is bigger than memory size" << std::endl;
        return false;
    }

    // step 2: 读取文件到 buffer
    auto content = new char[length];
    infile.read(content, (std::streamsize) length);

    // step 3: char -> unsigned char
    std::memcpy(&memory[pcStartMemory], content, length);
    std::cout << "ROM loaded, file size is: " << length << " Byte." << std::endl;
    delete[] content;

    return true;
}

void Memory::clear() {
    std::fill(std::begin(memory), std::end(memory), 0);
}

Byte &Memory::operator[](Word &address) {
    return memory[address];
}

void CPU::pcUpdate() {
    PC += 1;
}
