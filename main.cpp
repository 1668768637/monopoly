#include "monopolygame.h"
#include <QApplication>
#include <QLibrary>
#include <QtMultimedia/QAudioOutput>
#include <responseFunctions.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monopolyGame game;
    //心跳包
    game.socketThread->setResponseFunction("heartPackage",heartPackage);
    //登录结果
    game.socketThread->setResponseFunction("login",login);
    //匹配成功，游戏开始
    game.socketThread->setResponseFunction("beginGame",beginGame);
    //无用包
    game.socketThread->setResponseFunction("dontcare",dontcare);
    //玩家走一步
    game.socketThread->setResponseFunction("run",run);

    game.show();
    return a.exec();
}
