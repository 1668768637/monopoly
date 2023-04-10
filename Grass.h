#ifndef GRASS_H
#define GRASS_H
#include "AbstractMap.h"
#include "QObject"
#include "mapfactory.h"
class Grass:public AbstractMap
{
    Q_OBJECT
public:
    friend class MapFactory;
private:
    Grass();
};
#endif // GRASS_H
