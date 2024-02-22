#ifndef OPERABLEPLACE_H
#define OPERABLEPLACE_H
#include "AbstractMap.h"
#include "QObject"
#include <QMouseEvent>

class OperablePlace:public AbstractMap
{
    Q_OBJECT
public:
    void mousePressEvent(QMouseEvent* ev)
    {
        if (ev != nullptr && ev->button() == Qt::LeftButton)
        {
            emit clicked();    //emit signal
        }
    }

    void enterEvent(QEnterEvent*)
    {
        setCursor(Qt::PointingHandCursor);
    }
    void leaveEvent(QEvent*)
    {
        setCursor(Qt::ArrowCursor);
    }
    OperablePlace(int x,int y);
signals:
    void clicked();
};
#endif // OPERABLEPLACE_H
