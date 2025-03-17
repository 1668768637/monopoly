#ifndef OPERABLEPLACE_H
#define OPERABLEPLACE_H
#include "AbstractMap.h"
#include "QObject"
#include <QMouseEvent>

class OperablePlace:public AbstractMap
{
    Q_OBJECT
public:
    OperablePlace(int x,int y);

    void mousePressEvent(QMouseEvent* ev);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);
signals:
    void clicked();
};
#endif // OPERABLEPLACE_H
