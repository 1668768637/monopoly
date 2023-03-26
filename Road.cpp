#include "road.h"

bool Road::init()
{
    this->setPixmap(QPixmap(":/res/img/road.jpg"));
    return true;
}

Road::Road()
{
    setObjectName("Road");
}
