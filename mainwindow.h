#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *) override;

    void paintSmallSquare(int left, int top, QPainter &qpainter);

public:
    static constexpr int squareSize = 20;

    int windowWidth = 64 * squareSize;

    int windowHeight = 32 * squareSize;

signals:

};

#endif // MAINWINDOW_H
