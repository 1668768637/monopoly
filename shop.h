#ifndef SHOP_H
#define SHOP_H
#include "operableplace.h"
#include "mapfactory.h"
class Shop:public OperablePlace
{
    Q_OBJECT
public:
    friend class MapFactory;
private:
    Shop(int x,int y);

private slots:
    bool showShopUI();
};

#endif // SHOP_H
