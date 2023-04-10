#ifndef ROAD_H
#define ROAD_H
#include "AbstractMap.h"
#include "QObject"
#include "mapfactory.h"


class Road:public AbstractMap
{
    Q_OBJECT
public:
    friend class MapFactory;

    enum class Direct{LEFT,RIGHT,UP,DOWN};
    Direct direction;
    int stepCost;

private:
    Road(int type);

};
#endif // ROAD_H
