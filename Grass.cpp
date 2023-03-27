#include "Grass.h"

bool Grass::init()
{
    this->setPixmap(QPixmap(":/res/img/grass.png"));
    return true;
}

Grass::Grass()
{
    this->init();
}
