#include "UndefinedMap.h"


UndefinedMap::UndefinedMap(int x,int y):AbstractMap(x,y)
{
    this->setPixmap(QPixmap(":/res/img/noneBlock.png"));
}

bool UndefinedMap::init()
{
    return true;
}
