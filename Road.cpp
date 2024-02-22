#include "Road.h"

int Road::getStepCost()
{
    return stepCost;
}

bool Road::setStepCost(int stepCost)
{
    this->stepCost = stepCost;
    return true;
}

Road::Road(int type,int x,int y):AbstractMap(x,y)
{
    setObjectName("Road");
    //将自身大小设置为 36x51，与实际图片大小一致
    setFixedSize(36, 51);
    switch (type) {
    case 5:
        this->setPixmap(QPixmap(":/res/img/rightToUp.png"));
        this->direction = Direct::UP;
        this->stepCost = 1;
        break;
    case 6:
        this->setPixmap(QPixmap(":/res/img/downToRight.png"));
        this->direction = Direct::RIGHT;
        this->stepCost = 1;
        break;
    case 7:
        this->setPixmap(QPixmap(":/res/img/leftToDown.png"));
        this->direction = Direct::DOWN;
        this->stepCost = 1;
        break;
    case 8:
        this->setPixmap(QPixmap(":/res/img/upToLeft.png"));
        this->direction = Direct::LEFT;
        this->stepCost = 1;
        break;
    case 9:
        this->setPixmap(QPixmap(":/res/img/begin_right.png"));
        this->direction = Direct::RIGHT;
        this->stepCost = 1;
        break;
    case 10:
        this->setPixmap(QPixmap(":/res/img/HRoad.png"));
        this->direction = Direct::RIGHT;
        this->stepCost = 1;
        break;
    case 11:
        this->setPixmap(QPixmap(":/res/img/VRoad.png"));
        this->direction = Direct::DOWN;
        this->stepCost = 1;
        break;
    case 12:
        this->setPixmap(QPixmap(":/res/img/HRoad.png"));
        this->direction = Direct::LEFT;
        this->stepCost = 1;
        break;
    case 13:
        this->setPixmap(QPixmap(":/res/img/VRoad.png"));
        this->direction = Direct::UP;
        this->stepCost = 1;
        break;
    case 14:
        this->setPixmap(QPixmap(":/res/img/rightToDown.png"));
        this->direction = Direct::DOWN;
        this->stepCost = 1;
        break;
    case 15:
        this->setPixmap(QPixmap(":/res/img/downToLeft.png"));
        this->direction = Direct::LEFT;
        this->stepCost = 1;
        break;
    case 16:
        this->setPixmap(QPixmap(":/res/img/leftToUp.png"));
        this->direction = Direct::UP;
        this->stepCost = 1;
        break;
    case 17:
        this->setPixmap(QPixmap(":/res/img/upToRight.png"));
        this->direction = Direct::RIGHT;
        this->stepCost = 1;
        break;
    }
}

Road::Direct Road::getDirection() const
{
    return direction;
}

void Road::setDirection(Direct newDirection)
{
    direction = newDirection;
}
