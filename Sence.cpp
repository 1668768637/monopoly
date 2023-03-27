#include "sence.h"
#include <QPixmap>

Sence::Sence()
{
    setObjectName("Sence");
    this->init();
}

bool Sence::init()
{
    this->setPixmap(QPixmap(":/res/img/sence.jpg"));
    return true;
}
