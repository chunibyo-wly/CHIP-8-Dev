#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow{parent} {
    setFixedSize(Display::getScreenWidth(), Display::getScreenHeight());

    chip8Thread.start();
    connect(&chip8Thread, &CHIP8Thread::displaySignal, this, &MainWindow::updateScreen);
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    std::cout << "test" << std::endl;
}

void MainWindow::updateScreen(QImage &qimage) {
    pixmap = QPixmap::fromImage(qimage);
    update();
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);

    if (!pixmap.isNull()) {
        painter.drawPixmap(0, 0, pixmap);
    }
}
