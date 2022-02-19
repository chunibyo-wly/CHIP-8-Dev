#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow{parent} {
    setFixedSize(windowWidth, windowHeight);

    chi8Thread.start();
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
    paintSmallSquare(1, 2, qpainter);
    paintSmallSquare(2, 3, qpainter);
}

void MainWindow::paintSmallSquare(int left, int top, QPainter &qpainter) {
    qpainter.drawRect(QRectF(left * squareSize, top * squareSize, squareSize, squareSize));
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    std::cout << "test" << std::endl;
}
