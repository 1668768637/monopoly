#ifndef PLAYER_H
#define PLAYER_H
#include "Road.h"
#include"abstractplayer.h"
#include "knapsack.h"
#include "QPoint"

#define INIT_MONEY 20000

class Player:public AbstractPlayer
{
    Q_OBJECT
private:

public:
    Player(QPoint begin);

    static int num;
    int id;
    QString name;
    Knapsack* knapsack;
    QPoint gamemapPos;
    int steps;
signals:
    void playerChanged();
    void playerRun();
};
#endif // PLAYER_H

