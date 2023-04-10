#ifndef OPERABLEPLACE_H
#define OPERABLEPLACE_H
#include "AbstractMap.h"
#include "QObject"
#include <QMouseEvent>
#include "monopolygame.h"
#include "ui_monopolygame.h"

class OperablePlace:public AbstractMap
{
    Q_OBJECT
public:
    OperablePlace();
    monopolyGame *gameWindow;

    void mousePressEvent(QMouseEvent* ev)
    {
        if (ev != nullptr && ev->button() == Qt::LeftButton)
        {
            emit clicked();    //emit signal
        }
    }

    void enterEvent(QEvent*)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    void leaveEvent(QEvent*)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
};
#endif // OPERABLEPLACE_H
