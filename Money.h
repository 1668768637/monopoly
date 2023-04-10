#ifndef MONEY_H
#define MONEY_H

#include "prop.h"

class Money:public Prop
{
    Q_OBJECT
public:
    float num;
    Money(float num = 0.0);

signals:
    void moneyChanged();
};

#endif // MONEY_H
