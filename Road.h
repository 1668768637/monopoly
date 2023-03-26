#ifndef ROAD_H
#define ROAD_H
#include "AbstractMap.h"
#include "QObject"
class Road:public AbstractMap
{
    Q_OBJECT
public:
    bool init();
    Road();
};
#endif // ROAD_H
