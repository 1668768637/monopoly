#ifndef KNAPSACK_H
#define KNAPSACK_H
#include "Prop.h"
#include "QObject"
#include <QList>
#define KNASPACK_MAXLENGTH 64


class Knapsack:public Prop
{
    Q_OBJECT
public:
    Knapsack();
    bool addProp(Prop *prop);

    /**
     * @brief return the first Prop point in the container
     *
     * 函数详细描述
     *
     * @param className    a QString variable that describe the type you want to find
     * @return  return a point point to the first Prop you want to find
     */
    Prop* getProp(QString className);
private:
    static int maxLength;

    QList<Prop*> container;
};
#endif // KNAPSACK_H
