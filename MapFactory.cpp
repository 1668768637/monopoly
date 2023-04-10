#include "MapFactory.h"
#include "house.h"
#include "Grass.h"
#include "UndefinedMap.h"
#include "road.h"
#include "land.h"

AbstractMap* MapFactory::createMap(int type,int x,int y)
{
    AbstractMap *p;
    switch (type) {
    //草地
    case 1:
        p = new Grass();
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
        p = new Road(type);
        break;
    case 18:
        p = new Land();
        break;
    //房屋
    case 19:
        p = new House();
        break;
    default:
        p = new UndefinedMap();
        break;
    }
    p->gamemapPos.setX(x);
    p->gamemapPos.setY(y);
    return p;
}

AbstractMap *MapFactory::createMap(int type, QPoint position)
{
    return createMap(type,position.x(),position.y());
}
