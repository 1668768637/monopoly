#include "bank.h"
Bank::Bank()
{
    setObjectName("Bank");
}

bool Bank::option()
{
    return true;
}

bool Bank::init()
{
    this->setPixmap(QPixmap(":/res/img/house.jpg"));
    return true;
}
