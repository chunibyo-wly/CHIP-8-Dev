#include <iostream>
#include <QApplication>

#include "CHIP8.h"
#include "qt/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
