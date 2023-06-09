#ifndef SHOP_H
#define SHOP_H
#include "operableplace.h"
#include "mapfactory.h"
#include "Prop.h"
#include <Goods.h>

class Shop:public OperablePlace
{
    Q_OBJECT
public:
    friend class MapFactory;
    void refreshShop();
private:
    Shop(int x,int y);
    static QList<int> propCodeList;
    static QList<Goods*> goodsList;
    static int defaultListLength;

    QWidget* getPropUI();

private slots:
    bool showShopUI();
};

#endif // SHOP_H
