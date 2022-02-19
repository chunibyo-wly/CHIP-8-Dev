#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow{parent} {
    setFixedSize(windowWidth, windowHeight);

    chi8Thread.start();
    connect(&chi8Thread, &CHI8Thread::displaySignal, this, &MainWindow::updateScreen);
}

void MainWindow::paintEvent(QPaintEvent *) {
    resize(windowWidth, windowHeight);
    QPainter qpainter(this);

    QColor black("black");
    QColor white("white");
    qpainter.setBrush(black);
    qpainter.setPen(Qt::NoPen);
    qpainter.drawRect(0, 0, width(), height());

    qpainter.setBrush(white);

    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            if (_displayMatrix[i][j]) paintSmallSquare(j, i, qpainter);
        }
    }
}

void MainWindow::paintSmallSquare(int left, int top, QPainter &qpainter) {
    qpainter.drawRect(QRectF(left * squareSize, top * squareSize, squareSize, squareSize));
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    std::cout << "test" << std::endl;
}

void MainWindow::updateScreen(const Board &board) {
    // TODO: 优化：将Board变为指针传过来应该可以避免拷贝
    for (int i = 0; i < _height; ++i) {
        for (int j = 0; j < _width; ++j) {
            _displayMatrix[i][j] = board.data[i][j];
        }
    }
    update();
}
