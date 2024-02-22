#include "qpushbutton.h"
#include <responseFunctions.h>
#include <QDateTime>
#include <monopolygame.h>
#include <userinfogroup.h>
#include <QGroupBox>
#include <QLabel>
void heartPackage(monopolyGame *game, QJsonObject *msg)
{
    QDateTime sendDate = QDateTime::fromString(msg->value("sendDate").toString(), "yyyy-MM-dd hh:mm:ss.zzz");
    QDateTime respondDate(QDateTime::fromString(msg->value("respondDate").toString(), "yyyy-MM-dd hh:mm:ss.zzz"));
    int delay = sendDate.msecsTo(respondDate);
    emit game->socketThread->setCurrentDelay(delay);
}

void login(monopolyGame *game, QJsonObject *msg)
{
    //显示右上角用户信息
    if(msg->value("state").toString() == "OK")
    {
        game->ui->mainUI->findChild<QWidget*>("logWidget")->hide();
        game->ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->setEnabled(false);
        game->ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->hide();
        //显示右上角用户信息
        game->ui->mainUI->findChild<QGroupBox*>("userinfoGroup")->findChild<QWidget*>("layoutWidget")->findChild<QLabel*>("nickname")->setText(msg->value("nickname").toString());
        game->ui->mainUI->findChild<QGroupBox*>("userinfoGroup")->show();
    }
    else
    {
        game->ui->mainUI->findChild<QWidget*>("logWidget")->findChild<QLabel*>("logErrorMsgLabel")->setText(msg->value("error").toString());
    }
}

void beginGame(monopolyGame *game, QJsonObject *msg)
{
    qDebug()<<"游戏开始";
}
