#include "playerstatecontroller.h"

PlayerStateController::PlayerStateController()
{
    state = PlayerState::Normal;
}

bool PlayerStateController::isNormal()
{
    return state==PlayerState::Normal?true:false;
}

bool PlayerStateController::isBankruptcy()
{
    return state==PlayerState::Bankruptcy?true:false;
}

bool PlayerStateController::isActionable()
{
    for(int i = 0;i < affectList.length();i++)
    {
        if(affectList[i]->inherits("Sleepy"))//困倦效果，不能行动
        {
            return false;
        }
    }
    return true;
}

bool PlayerStateController::addAffect(Affect *affect)
{
    affectList.append(affect);
    return true;
}

bool PlayerStateController::refreshAffect(int currentRoound)
{
    for(int i = 0; i < affectList.length();i++)
    {
        Affect * affect = affectList.at(i);
        if(affect->beginRund+affect->lastRounds == currentRoound)
        {
            affectList.removeAt(i);
        }
    }
    return true;
}

bool PlayerStateController::cleanAffect()
{
    affectList.clear();
    return true;
}

PlayerState PlayerStateController::getState()
{
    return state;
}

bool PlayerStateController::setState(PlayerState state)
{
    this->state = state;
    return true;
}

