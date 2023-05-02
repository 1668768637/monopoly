#ifndef GAMEROUNDCONTROLLER_H
#define GAMEROUNDCONTROLLER_H
#include "controller.h"

class GameRoundController:public Controller
{
    Q_OBJECT
public:
    GameRoundController();
    int getRound();
    bool nextRound();
    bool reset();

signals:
    void valueChanged();

private:
    int round;

};

#endif // GAMEROUNDCONTROLLER_H
