#include "AbstractMap.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

AbstractMap::AbstractMap()
{
    setObjectName("AbstractMap");
    //将自身大小设置为 36x51，与实际图片大小一致
    setFixedSize(36, 51);
    setToolTip(QString::number(this->gamemapPos.x()) + "," + QString::number(this->gamemapPos.y()));
}


