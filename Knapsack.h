#ifndef KNAPSACK_H
#define KNAPSACK_H
#include "Prop.h"
#include "QObject"
class Knapsack:public Prop
{
    Q_OBJECT
private:
    Prop* contianer;
    int totalNum;
    int num;
public:
    Knapsack();
    int getTotalNum();
    int getNum();
};
#endif // KNAPSACK_H
