//
// Created by chunibyo on 2022/2/16.
//

#include "CHIP8.h"

void CHIP8::init() {
    // Most Chip-8 programs start at location 0x200 (512)
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1
    cpu.PC = Memory::pcStartMemory;
    std::fill(cpu.VRegister, cpu.VRegister + CPU::registerNum, 0);
    std::fill(cpu.stack, cpu.stack + 16, 0);
    cpu.SP = 0;
    cpu.IRegister = 0;
    cpu.delayTimer = 0;
    cpu.soundTimer = 0;
    cpu.memory.clear();
    for (int i = 0; i < 80; i++) {
        cpu.memory[i] = Display::chip8_fontset[i];
    }

    for (auto &i : _display->data)
        std::fill(i, i + 64, 0);
}

std::shared_ptr<Display> CHIP8::getDisplay() const {
    return _display;
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
        processOperationCode(operationCode);
    }
}

Word CHIP8::readOperationCode() {
    // All instructions are 2 bytes long
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.0
    return (cpu.memory[cpu.PC] << 8) | cpu.memory[cpu.PC + 1];
}

bool CHIP8::processOperationCode(Word &opcode) {
    // http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.1
    // https://github.com/trapexit/chip-8_documentation/blob/master/README.md
    int temp;
    auto &Vx = cpu.VRegister[(opcode >> 8) & 0x0F];
    auto &Vy = cpu.VRegister[(opcode >> 4) & 0x0F];
    auto &VF = cpu.VRegister[0xF];
    srand(time(nullptr));

    bool shouldVFChange = false;
    bool shouldUpdateScreen = false;

    cpu.PC += 2;
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode) {
                case 0x00E0:
                    for (auto &i : _display->data)
                        std::fill(i, i + 64, 0);
                    shouldUpdateScreen = true;
                    break;
                case 0x00EE:
                    // [00EE]
                    // The interpreter sets the program counter to the address at the top of the stack,
                    // then subtracts 1 from the stack pointer.
                    cpu.PC = cpu.stack[cpu.SP];
                    cpu.SP--;
                    break;
                default:
                    // TODO: Calls machine code routine
                    // [0nnn]
                    break;
            }
            break;
        case 0x1000:
            // [1nnn]
            // Jump to location nnn.
            // The interpreter sets the program counter to nnn.
            cpu.PC = (opcode & 0x0FFF);
            break;
        case 0x2000:
            // [2nnn]
            // Call subroutine at nnn.
            // The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            cpu.SP++;
            cpu.stack[cpu.SP] = cpu.PC;
            cpu.PC = (opcode & 0x0FFF);
            break;
        case 0x3000:
            // [3xnn]
            // Skip next instruction if Vx = kk.
            // The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
            if (Vx == (opcode & 0x00FF)) {
                cpu.PC += 2;
            }
            break;
        case 0x4000:
            // [4xnn]
            // Skip next instruction if Vx != kk.
            // The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
            if (Vx != (opcode & 0x00FF)) {
                cpu.PC += 2;
            }
            break;
        case 0x5000:
            // [5xy0]
            // Skip next instruction if Vx = Vy.
            // The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
            if (Vx == Vy) {
                cpu.PC += 2;
            }
            break;
        case 0x6000:
            // [6xkk]
            // Set Vx = kk.
            // The interpreter puts the value kk into register Vx.
            Vx = (opcode & 0x00FF);
            break;
        case 0x7000:
            // [7xkk]
            // Set Vx = Vx + kk.
            // Adds the value kk to the value of register Vx, then stores the result in Vx.
            Vx = (Vx + opcode) & 0x00FF;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    // [8xy0]
                    // Set Vx = Vy.
                    // Stores the value of register Vy in register Vx.
                    Vx = Vy;
                    break;
                case 0x0001:
                    // [8xy1]
                    // Set Vx = Vx OR Vy.
                    // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
                    // A bitwise OR compares the corresponding bits from two values, and if either bit is 1,
                    // then the same bit in the result is also 1. Otherwise, it is 0.
                    Vx |= Vy;
                    break;
                case 0x0002:
                    // [8xy2]
                    // Set Vx = Vx AND Vy.
                    // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
                    // A bitwise AND compares the corresponding bits from two values, and if both bits are 1,
                    // then the same bit in the result is also 1. Otherwise, it is 0.
                    Vx &= Vy;
                    break;
                case 0x0003:
                    // [8xy3]
                    // Set Vx = Vx XOR Vy.
                    Vx ^= Vy;
                    break;
                case 0x0004:
                    // [8xy4]
                    // Set Vx = Vx + Vy, set VF = carry.
                    // The values of Vx and Vy are added together.
                    // If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
                    // Only the lowest 8 bits of the result are kept, and stored in Vx.
                    VF = ((0xFF - Vx) < Vy);
                    Vx += Vy;
                    Vx &= 0xFF;
                    break;
                case 0x0005:
                    // [8xy5]
                    // Set Vx = Vx - Vy, set VF = NOT borrow.
                    // If Vx > Vy, then VF is set to 1, otherwise 0.
                    // Then Vy is subtracted from Vx, and the results stored in Vx.
                    VF = Vx > Vy;
                    Vx -= Vy;
                    Vx &= 0xFF;
                    break;
                case 0x0006:
                    // [8xy6]
                    // Set Vx = Vx SHR 1.
                    // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
                    // Then Vx is divided by 2.
                    VF = Vx & 0x0001;
                    Vx >>= 1;
                    break;
                case 0x0007:
                    // [8xy7]
                    // Set Vx = Vy - Vx, set VF = NOT borrow.
                    // If Vy > Vx, then VF is set to 1, otherwise 0.
                    // Then Vx is subtracted from Vy, and the results stored in Vx.
                    VF = Vy > Vx;
                    Vx = Vy - Vx;
                    Vx &= 0xFF;
                    break;
                case 0x000E:
                    // [8xyE]
                    // Set Vx = Vx SHL 1.
                    // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
                    // Then Vx is multiplied by 2.
                    VF = Vx >> 7;
                    Vx <<= 1;
                    Vx &= 0xFF;
                    break;
            }
            break;
        case 0x9000:
            // [9xy0]
            // Skip next instruction if Vx != Vy.
            // The values of Vx and Vy are compared, and if they are not equal,
            // the program counter is increased by 2.
            if (Vx != Vy) cpu.PC += 2;
            break;
        case 0xA000:
            // [Annn]
            // Set I = nnn.
            // The value of register I is set to nnn.
            cpu.IRegister = opcode & 0x0FFF;
            break;
        case 0xB000:
            // [Bnnn]
            // Jump to location nnn + V0.
            // The program counter is set to nnn plus the value of V0.
            cpu.PC = (opcode & 0x0FFF) + cpu.VRegister[0x0];
            break;
        case 0xC000:
            // [Cxkk]
            // Set Vx = random byte AND kk.
            // The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
            // The results are stored in Vx. See instruction 8xy2 for more information on AND.
            Vx = (rand() % 0xFF) & (opcode & 0xFF);
            break;
        case 0xD000:
            // [Dxyn]
            // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            // The interpreter reads n bytes from memory, starting at the address stored in I.
            // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
            // Sprites are XORed onto the existing screen. If this causes any pixels to be erased,
            // VF is set to 1, otherwise it is set to 0.
            // If the sprite is positioned so part of it is outside the coordinates of the display,
            // it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR,
            // and section 2.4, Display, for more information on the Chip-8 screen and sprites.
            temp = opcode & 0xF;
            for (int i = 0; i < temp; ++i) {
                auto &row = cpu.memory[cpu.IRegister + i];

                for (int j = 0; j < 8; ++j) {
                    if ((row & (0x80 >> j)) != 0) {
                        if (_display->data[Vy + i][Vx + j]) shouldVFChange = true;
                        _display->data[Vy + i][Vx + j] ^= 1;
                    }
                }
            }
            if (shouldVFChange) VF = 1; else VF = 0;
            shouldUpdateScreen = true;
            break;
        case 0xE000:
            switch (opcode & 0xF) {
                case 0xE:
                    if (Vx != 0) cpu.PC += 2;
                    break;
                case 0x1:
                    if (Vx == 0) cpu.PC += 2;
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0xFF) {
                case 0x07:
                    // [Fx07]
                    // Set Vx = delay timer value.
                    // The value of DT is placed into Vx.
                    Vx = cpu.delayTimer;
                    break;
                case 0x0A:
                    // [Fx0A]
                    // Wait for a key press, store the value of the key in Vx.
                    // All execution stops until a key is pressed, then the value of that key is stored in Vx.
                    // TODO
                    break;
                case 0x15:
                    // [Fx15]
                    // Set delay timer = Vx.
                    // DT is set equal to the value of Vx.
                    cpu.delayTimer = Vx;
                    break;
                case 0x18:
                    // [Fx18] 
                    // Set sound timer = Vx.
                    // ST is set equal to the value of Vx.
                    cpu.soundTimer = Vx;
                    break;
                case 0x1E:
                    // [Fx1E]
                    // Set I = I + Vx.
                    // The values of I and Vx are added, and the results are stored in I.
                    cpu.IRegister = cpu.IRegister + Vx;
                    break;
                case 0x29:
                    // [Fx29]
                    // Set I = location of sprite for digit Vx.
                    // The value of I is set to the location for the hexadecimal sprite
                    // corresponding to the value of Vx. See section 2.4, Display,
                    // for more information on the Chip-8 hexadecimal font.
                    // TODO
                    cpu.IRegister = Vx * 5;
                    break;
                case 0x33:
                    // [Fx33]
                    // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    // The interpreter takes the decimal value of Vx,
                    // and places the hundreds digit in memory at location in I,
                    // the tens digit at location I+1, and the ones digit at location I+2.
                    // I register does not change
                    cpu.memory[cpu.IRegister] = Vx / 100;
                    cpu.memory[cpu.IRegister + 1] = (Vx / 10) % 10;
                    cpu.memory[cpu.IRegister + 2] = Vx % 10;
                    break;
                case 0x55:
                    // [Fx55]
                    // Store registers V0 through Vx in memory starting at location I.
                    // The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
                    // I register dose not change
                    temp = (opcode & 0x0F00) >> 8;
                    for (size_t i = 0; i <= temp; ++i) {
                        cpu.memory[cpu.IRegister + i] = cpu.VRegister[i];
                    }
                    break;
                case 0x65:
                    // [Fx65]
                    // Read registers V0 through Vx from memory starting at location I.
                    // The interpreter reads values from memory starting at location I into registers V0 through Vx.
                    temp = (opcode & 0x0F00) >> 8;
                    for (size_t i = 0; i <= temp; ++i) {
                        cpu.VRegister[i] = cpu.memory[cpu.IRegister + i];
                    }
                    break;
            }
            break;
        default:
            std::cerr << "Unknown opcode!" << std::endl;
    }

    return shouldUpdateScreen;
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

Byte &Memory::operator[](Word address) {
    return memory[address];
}