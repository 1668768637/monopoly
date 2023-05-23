#ifndef BLACKHOLE_H
#define BLACKHOLE_H

#include "AbstractMap.h"
#include "mapfactory.h"

class BlackHole:public AbstractMap
{
    Q_OBJECT
public:
    friend class MapFactory;
private:
    BlackHole(int x,int y);
};

#endif // BLACKHOLE_H
