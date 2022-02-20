#include <QPainter>
#include "chip8thread.h"

CHIP8Thread::CHIP8Thread() {
    _display = chip8.getDisplay();
}

[[noreturn]] void CHIP8Thread::run() {
    chip8.init();
    std::string input = "./test_opcode.ch8";
    chip8.loadROM(input);

    while (true) {
        // operation code: 16 bit
        auto operationCode = chip8.readOperationCode();
        if (chip8.processOperationCode(operationCode)) {
            const auto &tmp = render();
            QImage qimage = std::move(*(render().get()));
            emit displaySignal(qimage);
        }

    }
}

std::shared_ptr<QImage> CHIP8Thread::render() {
    auto image = std::make_shared<QImage>(
            QSize(Display::getScreenWidth(), Display::getScreenHeight()), QImage::Format_RGB32);
    QPainter qpainter(image.get());

    QColor black("black");
    QColor white("white");
    qpainter.setBrush(black);
    qpainter.setPen(Qt::NoPen);
    qpainter.drawRect(0, 0, Display::getScreenWidth(), Display::getScreenHeight());

    qpainter.setBrush(white);

    for (int i = 0; i < Display::height; ++i) {
        for (int j = 0; j < Display::width; ++j) {
            if (_display->data[i][j]) paintSmallSquare(j, i, qpainter);
        }
    }
    return image;
}

void CHIP8Thread::paintSmallSquare(int left, int top, QPainter &qpainter) {
    qpainter.drawRect(
            QRectF(left * Display::squareSize, top * Display::squareSize,
                   Display::squareSize, Display::squareSize));
}