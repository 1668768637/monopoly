#ifndef LAND_H
#define LAND_H

#include "operableplace.h"
#include "mapfactory.h"

#define MAX_PRICE 8000
#define MIN_PRICE 4000

class Land: public OperablePlace
{
    Q_OBJECT
public:
    friend class MapFactory;

    float getPrice() const;
    void setPrice(float newPrice);

private:
    Land(int x,int y);
    float price;
private slots:
    bool showBuyHouseUI();
signals:
    void mapChanged();
};

#endif // LAND_H
