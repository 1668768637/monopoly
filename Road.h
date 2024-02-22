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

    enum class Direct{LEFT=1004,RIGHT,UP,DOWN};

    int getStepCost();
    bool setStepCost(int stepCost);

    Direct getDirection() const;
    void setDirection(Direct newDirection);

private:
    Road(int type,int x,int y);
    int stepCost;
    Direct direction;

};
#endif // ROAD_H
