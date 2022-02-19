#ifndef CHI8THREAD_H
#define CHI8THREAD_H
#include <memory>
#include <QThread>
#include <QImage>
#include <QPainter>
#include "CHIP8.h"

Q_DECLARE_METATYPE(Display);

class CHIP8Thread : public QThread {
Q_OBJECT

public:
    CHIP8Thread();

    CHIP8 chip8;

    std::shared_ptr<Display> _display;

    std::shared_ptr<QImage> render();

    void paintSmallSquare(int left, int top, QPainter &qpainter);

protected:
    [[noreturn]] void run() override;

signals:

    void displaySignal(QImage &qimage);
};

#endif // CHI8THREAD_H
