#ifndef MONEY_H
#define MONEY_H

#include "prop.h"

class Money:public Prop
{
    Q_OBJECT
public:
    Money(float num = 0.0);
    bool use(Player *targetPlayer,int currentRound);
    float getNum();
    bool add(float num);
    bool reduce(float num);
    bool setNum(float num);
private:
    float num;

signals:
    void moneyChanged();
};

#endif // MONEY_H
