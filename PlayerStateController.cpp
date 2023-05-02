#include "playerstatecontroller.h"

PlayerStateController::PlayerStateController()
{
    state = playerState::Normal;
}

bool PlayerStateController::isNormal()
{
    return state==playerState::Normal?true:false;
}

bool PlayerStateController::isFailed()
{
    return state==playerState::Failed?true:false;
}

playerState PlayerStateController::getState()
{
    return state;
}

bool PlayerStateController::setState(playerState state)
{
    this->state = state;
    return true;
}

