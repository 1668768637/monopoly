#ifndef MAPFACTORY_H
#define MAPFACTORY_H
#include "AbstractMap.h"
#include "monopolygame.h"
#include <QPoint>

class monopolyGame;

class MapFactory
{
public:
    static AbstractMap* createMap(int type,int x,int y);
    static AbstractMap* createMap(int type,QPoint position);
};
#endif // MAPFACTORY_H
