#include "shop.h"


Shop::Shop(int x,int y):OperablePlace(x,y)
{
    setObjectName("Shop");
    //将自身大小设置为 36x51，与实际图片大小一致
    setFixedSize(36, 51);
    this->setPixmap(QPixmap(":/res/img/shop.png"));

    connect(this,SIGNAL(clicked()),this,SLOT(showShopUI()));
}

bool Shop::showShopUI()
{
    return true;
}
