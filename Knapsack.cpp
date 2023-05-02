#include "Knapsack.h"

int Knapsack::maxLength = KNASPACK_MAXLENGTH;

Knapsack::Knapsack()
{
    setObjectName("Knapsack");
}

bool Knapsack::addProp(Prop *prop)
{
    if(container.length()+1 > maxLength)
        return false;
    else
    {
        container.append(prop);
        return true;
    }
}

Prop* Knapsack::getProp(QString className)
{
    Prop* obj = nullptr;
    for(int propNum = 0;propNum < container.length();propNum++)
    {
        if(container.at(propNum)->inherits(className.toLatin1().data()))
        {
            obj = container.at(propNum);
        }
    }

    return obj;
}
