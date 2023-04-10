#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QLabel>
#include <QWidget>
#include "QObject"
class monopolyGame;

class GameObjcet:public QLabel
{
    Q_OBJECT
public:
    GameObjcet();
};
#endif // GAMEOBJECT_H
