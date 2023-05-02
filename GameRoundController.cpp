#include "gameroundcontroller.h"

GameRoundController::GameRoundController()
{
    round = 1;
}

int GameRoundController::getRound()
{
    return round;
}

bool GameRoundController::nextRound()
{
    round++;
    emit valueChanged();
    return true;
}

bool GameRoundController::reset()
{
    round = 1;
    return true;
}
