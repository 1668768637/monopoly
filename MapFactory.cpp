#include "MapFactory.h"
#include "gameobject.h"
#include "house.h"
#include "Grass.h"
#include "UndefinedMap.h"
#include "road.h"
#include "land.h"
#include "shop.h"
#include "blackhole.h"

AbstractMap* MapFactory::createMap(int type,int x,int y,GameObjcet *obj)
{
    AbstractMap *p;
    switch (type) {
    //草地
    case 1:
        p = new Grass(x,y);
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
        p = new Road(type,x,y);
        break;
    case 18:
        p = new Land(x,y);
        break;
    //房屋
    case 19:
        p = new House(dynamic_cast<Player*>(obj),x,y);
        break;
    //黑洞
    case 21:
        p = new BlackHole(x,y);
        break;
    //超市
    case 27:
        p = new Shop(x,y);
        break;
    default:
        p = new UndefinedMap(x,y);
        break;
    }
    return p;
}

AbstractMap *MapFactory::createMap(int type, QPoint position,GameObjcet *obj)
{
    return createMap(type,position.x(),position.y(),obj);
}
