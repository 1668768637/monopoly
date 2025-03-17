#include "operableplace.h"

OperablePlace::OperablePlace(int x,int y):AbstractMap(x,y)
{
    setObjectName("OperablePlace");
}

void OperablePlace::mousePressEvent(QMouseEvent *ev)
{
    if (ev != nullptr && ev->button() == Qt::LeftButton)
    {
        emit clicked();    //emit signal
    }
}

void OperablePlace::enterEvent(QEnterEvent *)
{
    setCursor(Qt::PointingHandCursor);
}

void OperablePlace::leaveEvent(QEvent *)
{
    setCursor(Qt::ArrowCursor);
}
