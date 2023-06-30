#include "Knapsack.h"
#include<QDebug>
#include <sleepycard.h>

int Knapsack::maxLength = KNASPACK_MAXLENGTH;

Knapsack::Knapsack():Prop(PropType::unShow)
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

bool Knapsack::removeProp(Prop *prop)
{
    container.removeOne(prop);
    return true;
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

bool Knapsack::use(Player *targetPlayer, int currendRound)
{
    Q_UNUSED(targetPlayer)
    Q_UNUSED(currendRound)
    return true;
}

bool Knapsack::showRequestVarUI()
{
    return true;
}

QWidget *Knapsack::getTab()
{
    QWidget* returnWidget = new QWidget();
    int index = 0,propNum = 0;

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setSpacing(50);
    hl->setContentsMargins(50,0,0,0);
    while(index < container.length())
    {
        if(propNum%10==0 && propNum!=0)//一行满了
        {
            vl->addLayout(hl);
            hl = new QHBoxLayout();
            hl->setSpacing(50);
            hl->setContentsMargins(50,0,0,0);
        }


        Prop* prop = container.at(index);
        if(prop->type != PropType::unShow)
        {
            hl->addWidget(prop);
            propNum++;
        }
        index++;
    }

    hl->addStretch();
    vl->addLayout(hl);//加上最后一个没有满的
    returnWidget->setLayout(vl);
    return returnWidget;
}

QWidget *Knapsack::getTab(PropType type)
{
    QWidget* returnWidget = new QWidget();
    int index = 0,propNum = 0;

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setSpacing(50);
    hl->setContentsMargins(50,0,0,0);
    while(index < container.length())
    {
        if(propNum%10==0 && propNum!=0)//一行满了
        {
            vl->addLayout(hl);
            hl = new QHBoxLayout();
            hl->setSpacing(50);
            hl->setContentsMargins(50,0,0,0);
        }


        Prop* prop = container.at(index);

        if(prop->type == type)
        {
            hl->addWidget(prop);
            propNum++;
        }
        index++;
    }

    hl->addStretch();
    vl->addLayout(hl);//加上最后一个没有满的
    returnWidget->setLayout(vl);
    return returnWidget;
}

