#include "road.h"

bool Road::init()
{
    this->setPixmap(QPixmap(":/res/img/road.png"));
    return true;
}

Road::Road()
{
    setObjectName("Road");
    this->init();
}
