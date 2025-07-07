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

    void updateScreen(const QImage &qimage);

public:

    CHIP8Thread chip8Thread;

    QPixmap pixmap;

signals:

};

#endif // MAINWINDOW_H
