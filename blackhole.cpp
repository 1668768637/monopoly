#include "blackhole.h"

BlackHole::BlackHole(int x,int y):AbstractMap(x,y)
{
    setPixmap(QPixmap(":/res/img/blackHole.png"));
}
