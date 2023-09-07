#include "money.h"

Money::Money(float num):Prop(PropType::unShow)
{
    this->num = num;
}

bool Money::use(Player *targetPlayer, int currentRound)
{
    Q_UNUSED(targetPlayer)
    Q_UNUSED(currentRound)
    return true;
}

bool Money::showRequestVarUI()
{
    return true;;
}

float Money::getNum()
{
    return num;
}

bool Money::add(float num)
{
    this->num+=num;
    emit moneyChanged();
    return true;
}

bool Money::reduce(float num)
{
    this->num-=num;
    emit moneyChanged();
    return true;
}

bool Money::setNum(float num)
{
    this->num = num;
    emit moneyChanged();
    return true;
}

