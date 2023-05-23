#include "AbstractMap.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

AbstractMap::AbstractMap(int x,int y)
{
    setObjectName("AbstractMap");
    //将自身大小设置为 36x51，与实际图片大小一致
    setFixedSize(36, 51);
    gamemapPos.setX(x);
    gamemapPos.setY(y);

#ifdef DEBUG
    this->setToolTip(QString::number(gamemapPos.x()) + "," + QString::number(gamemapPos.y()));
#endif
}


