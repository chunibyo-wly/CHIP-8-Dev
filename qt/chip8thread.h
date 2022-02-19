#ifndef CHI8THREAD_H
#define CHI8THREAD_H

#include <QThread>
#include "CHIP8.h"

class CHI8Thread : public QThread
{
public:
    CHI8Thread();

protected:
    void run() override;
};

#endif // CHI8THREAD_H
