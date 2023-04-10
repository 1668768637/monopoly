#ifndef HOUSE_H
#define HOUSE_H
#include "OperablePlace.h"
#include "QObject"
#include "mapfactory.h"
#include "player.h"

class House:public OperablePlace
{
    Q_OBJECT
public:
    friend class MapFactory;
    Player *owner;
    float rent;

private:
    House();

private slots:
    bool showHouseInfo();

};
#endif // HOUSE_H
