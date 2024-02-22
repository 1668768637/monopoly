#include "Player.h"
#include "Money.h"


int Player::num = 0;

int Player::getId() const
{
    return id;
}

void Player::setId(int newId)
{
    id = newId;
}

QString Player::getName() const
{
    return name;
}

void Player::setName(const QString &newName)
{
    name = newName;
}

int Player::getSteps() const
{
    return steps;
}

void Player::setSteps(int newSteps)
{
    steps = newSteps;
}

Knapsack *Player::getKnapsack() const
{
    return knapsack;
}

void Player::setKnapsack(Knapsack *newKnapsack)
{
    knapsack = newKnapsack;
}

QPoint &Player::getGamemapPos()
{
    return gamemapPos;
}

void Player::setGamemapPos(QPoint newGamemapPos)
{
    gamemapPos = newGamemapPos;
}

PlayerStateController *Player::getStateController() const
{
    return stateController;
}


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
    this->stateController = new PlayerStateController();

    this->knapsack->addProp(new Money(INIT_MONEY));
    dynamic_cast<Money*>(this->knapsack->getProp("Money"))->setNum(money);
}
