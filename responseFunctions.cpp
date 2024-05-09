#include "qjsonarray.h"
#include "qpushbutton.h"
#include <responseFunctions.h>
#include <QDateTime>
#include <monopolygame.h>
#include <userinfogroup.h>
#include <QGroupBox>
#include <QLabel>
#include <QFile>
#include <QDir>
void heartPackage(monopolyGame *game, QJsonObject *msg)
{
    QDateTime sendDate = QDateTime::fromString(msg->value("sendDate").toString(), "yyyy-MM-dd hh:mm:ss.zzz");
    QDateTime respondDate(QDateTime::fromString(msg->value("respondDate").toString(), "yyyy-MM-dd hh:mm:ss.zzz"));
    int delay = sendDate.msecsTo(respondDate);
    emit game->socketThread->setCurrentDelay(delay);
}

void login(monopolyGame *game, QJsonObject *msg)
{
    //登录成功
    if(msg->value("state").toString() == "OK")
    {
        game->ui->mainUI->findChild<QWidget*>("logWidget")->hide();
        game->ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->setEnabled(false);
        game->ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->hide();
        //显示右上角用户信息
        game->ui->mainUI->findChild<QGroupBox*>("userinfoGroup")->findChild<QLabel*>("nickname")->setText(msg->value("nickname").toString());
        game->ui->mainUI->findChild<QGroupBox*>("userinfoGroup")->show();

        //激活联机游戏按钮
        game->ui->mainUI->findChild<QPushButton*>("startGameBtn_CS")->setEnabled(true);
        game->myID = msg->value("ID").toString();
    }
    else
    {
        game->ui->mainUI->findChild<QWidget*>("logWidget")->findChild<QLabel*>("logErrorMsgLabel")->setText(msg->value("error").toString());
    }
}

void beginGame(monopolyGame *game, QJsonObject *msg)
{
    qDebug()<<"游戏开始";
    game->isCSGame = true;

    //解析地图文件，房间信息
    QJsonArray fileList = msg->value("fileList").toArray();
    QString prePath = msg->value("path").toString();
    for(int i = 0;i < fileList.count();i++)
    {
        QString filepath = prePath + "/" + fileList.at(i).toString();
        QFile file(filepath);
        if(!QDir().exists(prePath))
        {
            QDir().mkdir(prePath);
        }
        if(file.exists())
        {
            //删除旧的配置文件
            file.remove();
        }
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(msg->value(fileList.at(i).toString()).toString().toStdString().data());
            file.close();
        }
        else
        {
            qDebug()<<"下载游戏核心文件时打开文件失败."<<file.errorString();
        }
    }
    emit game->beginGame(true);
}

void dontcare(monopolyGame *game, QJsonObject *msg)
{
    Q_UNUSED(game)
    Q_UNUSED(msg)
}

void run(monopolyGame *game, QJsonObject *msg)
{
    int steps = msg->value("steps").toInt();
    QString runPlayerID = msg->value("runPlayerID").toString();

    if(game->runningPlayer->getId() == runPlayerID)
    {
        emit game->socketThread->playerRun(steps);
    }

}
