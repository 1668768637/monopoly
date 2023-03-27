#ifndef ROAD_H
#define ROAD_H
#include "AbstractMap.h"
#include "QObject"
#include "mapfactory.h"
class Road:public AbstractMap
{
    Q_OBJECT
public:
    bool init();
    friend class MapFactory;
private:
    Road();
};
#endif // ROAD_H
