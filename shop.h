#ifndef SHOP_H
#define SHOP_H
#include "operableplace.h"
#include "mapfactory.h"
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
    /**
     * @brief registe propcode
     *
     *  registe a propcode to shop so that ensure special prop could be construct
     *
     * @param propCode  an special code which represent a prop,please ensure the code is available
     * @return void
     */
    void registeProp(int propCode);
    //随机生成道具
    void generateGoods();
    //随机生成道具
    void generateGoods(int nums);

private slots:
    bool showShopUI();
signals:
};

#endif // SHOP_H
