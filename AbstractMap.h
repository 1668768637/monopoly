#ifndef ABSTRACTMAP_H
#define ABSTRACTMAP_H
#include "GameObject.h"
#include "QObject"
#include <QPoint>

class AbstractMap:public GameObjcet
{
    Q_OBJECT
public:
    friend class MapFactory;

    QPoint gamemapPos;
protected:
    AbstractMap();
signals:
    void clicked();
private:
};
#endif // ABSTRACTMAP_H
