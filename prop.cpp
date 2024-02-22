#include "prop.h"
#include "monopolygame.h"
#include <QScrollArea>

PropType Prop::getType() const
{
    return type;
}

void Prop::setType(PropType newType)
{
    type = newType;
}

float Prop::getPrice() const
{
    return price;
}

void Prop::setPrice(float newPrice)
{
    price = newPrice;
}

Prop::Prop(PropType t):type(t),price(0.0)
{
    setFixedSize(QSize(90,140));
    setMinimumSize(QSize(90,140));

    connect(this,&Prop::clicked,this,showRequestVarUI);
    connect(this,&Prop::used,this,[=](){
            gameWindow->ui->gamePannel->findChild<QGroupBox*>("knapsackUI")->findChild<QGroupBox*>("bodyBox")->findChild<QScrollArea*>("propList")->setWidget(gameWindow->runningPlayer->getKnapsack()->getTab());
    });
}
