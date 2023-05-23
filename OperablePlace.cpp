#include "operableplace.h"

OperablePlace::OperablePlace(int x,int y):AbstractMap(x,y)
{
    setObjectName("OperablePlace");
    gameWindow = monopolyGame::myWindow;
}
