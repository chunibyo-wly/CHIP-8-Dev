#include <iostream>
#include <QApplication>

#include "CHIP8.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
//    CHIP8 chip8;
//    chip8.init();
//    std::string input = "/Users/chunibyo/01_project/06_open_source/19_CHIP8/test_opcode.ch8";
//    chip8.loadROM(input);
//    chip8.run();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
