#ifndef MONEY_H
#define MONEY_H
#include "Prop.h"
#include "QObject"
class Money:public Prop
{
    Q_OBJECT
private:
    int nums;
public:
    Money();
    int getNum();
};
#endif // MONEY_H
