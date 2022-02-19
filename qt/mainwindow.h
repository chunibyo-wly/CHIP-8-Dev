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

    void updateScreen(const Board &board);

public:
    static constexpr int squareSize = 20;

    static constexpr int _width = 64;

    static constexpr int _height = 32;

    int windowWidth = _width * squareSize;

    int windowHeight = _height * squareSize;

    CHI8Thread chi8Thread;

    bool _displayMatrix[_height][_width] = {false};

signals:

};

#endif // MAINWINDOW_H
