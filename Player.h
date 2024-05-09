#ifndef PLAYER_H
#define PLAYER_H
#include"abstractplayer.h"
#include "knapsack.h"
#include "QPoint"
#include "playerstatecontroller.h"
#include <Money.h>

#define INIT_MONEY 20000

class Player:public AbstractPlayer
{
    Q_OBJECT
private:
    static int num;
    QString id;
    QString name;
    QPoint gamemapPos;
    int steps;
    Knapsack* knapsack;
    PlayerStateController *stateController;

public:
    Player(QPoint begin,float money);

    QString getId() const;
    void setId(QString newId);

    QString getName() const;
    void setName(const QString &newName);


    int getSteps() const;
    void setSteps(int newSteps);

    Knapsack *getKnapsack() const;
    void setKnapsack(Knapsack *newKnapsack);

    QPoint& getGamemapPos();

    PlayerStateController *getStateController() const;
    void setStateController(PlayerStateController *newStateController);

    void setGamemapPos(QPoint newGamemapPos);

signals:
    void playerChanged();
    void playerRun();
};
#endif // PLAYER_H

