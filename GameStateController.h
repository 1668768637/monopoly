#ifndef GAMESTATECONTROLLER_H
#define GAMESTATECONTROLLER_H
#include "controller.h"

enum class gameState{Ready=1001,Running,End};

class GameStateController:public Controller
{
    Q_OBJECT
public:
    GameStateController();
    bool isReady();
    bool isRunning();
    bool isEnd();
    gameState getState();
    bool setState(gameState state);

private:
    gameState state;
};

#endif // GAMESTATECONTROLLER_H
