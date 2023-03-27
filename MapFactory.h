#ifndef MAPFACTORY_H
#define MAPFACTORY_H
#include "AbstractMap.h"


class MapFactory
{
public:
    static AbstractMap* createMap(int type);
};
#endif // MAPFACTORY_H
