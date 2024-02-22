#include "monopolygame.h"
#include <QApplication>
#include <QLibrary>
#include <QtMultimedia/QAudioOutput>
#include <responseFunctions.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monopolyGame game;
    //登录成功
    game.socketThread->setResponseFunction("heartPackage",heartPackage);
    //登录失败
    game.socketThread->setResponseFunction("login",login);
    //匹配成功，游戏开始
    game.socketThread->setResponseFunction("beginGame",beginGame);
    game.show();
    return a.exec();
}
