#ifndef PLAYERSTATECONTROLLER_H
#define PLAYERSTATECONTROLLER_H
#include "controller.h"
#include "Affect.h"

enum class PlayerState{Normal=1010,Bankruptcy};

class PlayerStateController:public Controller
{
    Q_OBJECT
private:
    PlayerState state;
    QList<Affect*> affectList;
public:
    PlayerStateController();

    bool isNormal();
    bool isBankruptcy();
    bool isActionable();
    bool addAffect(Affect* affect);
    bool refreshAffect(int currentRoound);
    bool cleanAffect();
    PlayerState getState();
    bool setState(PlayerState state);
};

#endif // PLAYERSTATECONTROLLER_H
