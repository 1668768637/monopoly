#include "sleepycard.h"
#include "player.h"

SleepCard::SleepCard():Prop(PropType::control)
{
    affect = new Sleepy(-1);
    this->setPixmap(QPixmap(":/res/img/sleepCard.png"));
}

bool SleepCard::use(Player *targetPlayer,int currentRound)
{
    affect->beginRund = currentRound;
    targetPlayer->stateController.addAffect(affect);
    return true;
}

