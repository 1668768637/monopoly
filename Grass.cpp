#include "Grass.h"


Grass::Grass(int x,int y):AbstractMap(x,y)
{
    this->setPixmap(QPixmap(":/res/img/grass.png"));
}
