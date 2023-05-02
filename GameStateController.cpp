#include "gamestatecontroller.h"

GameStateController::GameStateController()
{
    state=gameState::Ready;
}

bool GameStateController::isReady()
{
    return state==gameState::Ready?true:false;
}

bool GameStateController::isRunning()
{
    return state==gameState::Running?true:false;
}


bool GameStateController::isEnd()
{
    return state==gameState::End?true:false;
}

gameState GameStateController::getState()
{
    return state;
}

bool GameStateController::setState(gameState state)
{
    this->state = state;
    return true;
}

