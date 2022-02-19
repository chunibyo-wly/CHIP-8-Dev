#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QPainter>
#include <QTimer>

#include "CHIP8.h"
#include "chip8thread.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *) override;

    void mousePressEvent(QMouseEvent *e) override;

    void paintSmallSquare(int left, int top, QPainter &qpainter);

public:
    static constexpr int squareSize = 20;

    int windowWidth = 64 * squareSize;

    int windowHeight = 32 * squareSize;

    CHI8Thread chi8Thread;

signals:

};

#endif // MAINWINDOW_H
