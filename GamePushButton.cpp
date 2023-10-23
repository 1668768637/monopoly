#include <GamePushButton.h>


GamePushButton::GamePushButton(QWidget *parent):QPushButton(parent)
{
    this->setFocusPolicy(Qt::NoFocus);
}

GamePushButton::~GamePushButton()
{

}
