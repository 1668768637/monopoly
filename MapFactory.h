#ifndef MAPFACTORY_H
#define MAPFACTORY_H
#include "AbstractMap.h"
#include "monopolygame.h"
#include <QPoint>

class monopolyGame;

class MapFactory
{
public:
    static AbstractMap* createMap(int type,int x,int y,GameObjcet *obj = nullptr);
    static AbstractMap* createMap(int type,QPoint position,GameObjcet *obj = nullptr);
};
#endif // MAPFACTORY_H
