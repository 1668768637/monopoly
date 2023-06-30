#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QLabel>
#include <QWidget>
#include "QObject"
#include "ui_monopolygame.h"
class monopolyGame;

class GameObjcet:public QLabel
{
    Q_OBJECT
public:
    GameObjcet();
    monopolyGame *gameWindow;
};
#endif // GAMEOBJECT_H
