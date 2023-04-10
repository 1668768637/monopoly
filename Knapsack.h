#ifndef KNAPSACK_H
#define KNAPSACK_H
#include "Prop.h"
#include "QObject"
#include <QList>
class Prop;
class Knapsack:public Prop
{
    Q_OBJECT
public:
    Knapsack();
    QList<Prop*> container;
};
#endif // KNAPSACK_H
