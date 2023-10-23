#ifndef GOODS_H
#define GOODS_H

#include <gameobject.h>
#include <Prop.h>

class Goods:public GameObjcet
{
    Q_OBJECT
public:
    Goods(Prop *p);
private:
    Prop *prop;
    float price;
signals:
    void hasBeenBought();
};

#endif // GOODS_H
