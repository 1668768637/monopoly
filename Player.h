#ifndef PLAYER_H
#define PLAYER_H
#include"abstractplayer.h"
#include "knapsack.h"
#include "QPoint"
#include "playerstatecontroller.h"

#define INIT_MONEY 20000

class Player:public AbstractPlayer
{
    Q_OBJECT
private:
    static int num;

public:
    Player(QPoint begin,float money);

    int id;
    QString name;
    QPoint gamemapPos;
    int steps;
    Knapsack* knapsack;
    PlayerStateController stateController;
signals:
    void playerChanged();
    void playerRun();
};
#endif // PLAYER_H

