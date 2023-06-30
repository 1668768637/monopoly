#include "Player.h"
#include "AbstractMap.h"
#include "money.h"


int Player::num = 0;

Player::Player(QPoint begin,float money)
{
    setObjectName("Player");
    //将自身大小设置为 36x51，与实际图片大小一致
    setFixedSize(36, 51);
    this->knapsack = new Knapsack();
    this->id = ++num;
    this->name = QString("player") + QString::number(id);
    gamemapPos.setX(begin.x());
    gamemapPos.setY(begin.y());
    this->setPixmap(QPixmap(":/res/img/people.png"));
    this->steps = 0;

    this->knapsack->addProp(new Money(INIT_MONEY));
    dynamic_cast<Money*>(this->knapsack->getProp("Money"))->setNum(money);
}
