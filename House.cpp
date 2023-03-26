#include "house.h"

House::House()
{
    setObjectName("House");
}

bool House::option()
{
    return true;
}

bool House::init()
{
    this->setPixmap(QPixmap(":/res/img/house.png"));
    return true;
}
