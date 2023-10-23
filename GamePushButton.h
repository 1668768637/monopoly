#ifndef GAMEPUSHBUTTON_H
#define GAMEPUSHBUTTON_H

#include <QPushButton>

class GamePushButton:public QPushButton
{
    Q_OBJECT
public:
    GamePushButton(QWidget *parent = nullptr);
    ~GamePushButton();

};

#endif // GAMEPUSHBUTTON_H
