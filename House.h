#ifndef HOUSE_H
#define HOUSE_H
#include "OperablePlace.h"
#include "QObject"
#include "mapfactory.h"

class House:public OperablePlace
{
    Q_OBJECT
public:
    friend class MapFactory;
    bool option();
    bool init();
private:
    House();
};
#endif // HOUSE_H
