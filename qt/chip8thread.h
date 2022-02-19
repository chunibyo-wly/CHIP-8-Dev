#ifndef CHI8THREAD_H
#define CHI8THREAD_H

#include <QThread>
#include "CHIP8.h"

struct Board {
    int data[32][64] = {false};
};

Q_DECLARE_METATYPE(Board);

class CHI8Thread : public QThread {
Q_OBJECT

public:
    CHI8Thread();

protected:
    [[noreturn]] void run() override;

signals:

    void displaySignal(const Board &displayMatrix);
};

#endif // CHI8THREAD_H
