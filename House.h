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

    bool setRent(float runt);
    float getRent() const;

    Player *getOwner() const;
    void setOwner(Player *newOwner);

private:
    Player *owner;
    float rent;
    House(Player *owner,int x,int y);

private slots:
    bool showHouseInfo();

};
#endif // HOUSE_H
