#include "MapFactory.h"
#include "house.h"
#include "Grass.h"
#include "UndefinedMap.h"
#include "road.h"

AbstractMap* MapFactory::createMap(int type)
{
    switch (type) {
    //草地
    case 1:
        return new Grass();
    //房屋
    case 19:
        return new House();
    case 10:
        return new Road();
    default:
        return new UndefinedMap();
    }
}
