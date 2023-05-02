#ifndef PLAYERSTATECONTROLLER_H
#define PLAYERSTATECONTROLLER_H
#include "controller.h"

enum class playerState{Normal=1010,Failed};

class PlayerStateController:public Controller
{
    Q_OBJECT
private:
    playerState state;
public:
    PlayerStateController();

    bool isNormal();
    bool isFailed();
    playerState getState();
    bool setState(playerState state);
};

#endif // PLAYERSTATECONTROLLER_H
