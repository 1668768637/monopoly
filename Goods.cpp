#include "Goods.h"
#include "monopolygame.h"
#include <ui_monopolygame.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPushButton>


Goods::Goods(Prop *p)
{
    this->setFixedSize(120,200);


    this->setStyleSheet("border:2px solid;");
    prop = p;
    price = p->price;
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* iconLine = new QHBoxLayout();
    QLabel* icon = new QLabel();
    icon->setPixmap(p->pixmap());
    iconLine->addWidget(icon,Qt::AlignCenter);

    QHBoxLayout* operationLine = new QHBoxLayout();
    QPushButton* buyBtn = new QPushButton();
    buyBtn->setText("购买");
    connect(buyBtn,&QPushButton::clicked,this,[=](){
        gameWindow->runningPlayer->knapsack->addProp(p);
        Money* money = dynamic_cast<Money*>(gameWindow->runningPlayer->knapsack->getProp("Money"));
        money->reduce(p->price);
        emit hasBeenBought();
    });
    operationLine->addWidget(buyBtn,Qt::AlignCenter);


    layout->addLayout(iconLine);
    layout->addLayout(operationLine);
    this->setLayout(layout);
}
