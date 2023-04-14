#include "land.h"
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include "money.h"
#include <QMessageBox>
#include "house.h"


Land::Land()
{
    setPixmap(QPixmap(":/res/img/land.png"));
    QObject::connect(this,SIGNAL(clicked()),this,SLOT(showBuyHouseUI()));
    QObject::connect(this,&Land::mapChanged,[=](){
        gameWindow->printMap();
    });

    price = qrand()%(MAX_PRICE - MIN_PRICE) + MIN_PRICE;
}

bool Land::showBuyHouseUI()
{
    //防止重复出现，导致游戏逻辑出现问题
    if(gameWindow->ui->gamePannel->findChild<QGroupBox*>("buyPannel") != nullptr)return false;

    QPoint senderPos = dynamic_cast<Land*>(sender())->gamemapPos;
    Land *land = dynamic_cast<Land*>(gameWindow->mapList[senderPos.x()][senderPos.y()]);

    int buyPannel_width = 300,buyPannel_height = 200;
    QGroupBox *buyPannel = new QGroupBox(gameWindow->ui->gamePannel);
    buyPannel->setGeometry(gameWindow->ui->gamePannel->width()/2-buyPannel_width/2,gameWindow->ui->gamePannel->height()/2-buyPannel_height/2,buyPannel_width,buyPannel_height);
    buyPannel->setObjectName("buyPannel");

    QLabel *priceLab = new QLabel(buyPannel);
    priceLab->setText("当前房价：" + QString::number(land->price));
    QPushButton *confirm = new QPushButton(buyPannel);
    confirm->setText("购买房屋");
    QPushButton *cancle = new QPushButton(buyPannel);
    cancle->setText("取消购买");

    priceLab->setStyleSheet("font-size:20px;");

    priceLab->move(70,50);
    confirm->move(50,buyPannel->height()-50);
    cancle->move(175,buyPannel->height()-50);

    //取消购买
    connect(cancle,&QPushButton::clicked,[=](){
        buyPannel->close();
        delete confirm;
        delete cancle;
        delete priceLab;
        delete buyPannel;
    });

    //确认购买
    connect(confirm,&QPushButton::clicked,[=](){
        buyPannel->close();
        delete confirm;
        delete cancle;
        delete priceLab;
        delete buyPannel;


        //获取距离
        int distance;
        Road *road = dynamic_cast<Road*>(gameWindow->mapList[gameWindow->runningPlayer->gamemapPos.x()][gameWindow->runningPlayer->gamemapPos.y()]);
        if(road->direction == Road::Direct::DOWN || road->direction == Road::Direct::UP)
        {
            if(gameWindow->runningPlayer->gamemapPos.x() == senderPos.x())
                distance = fabs(gameWindow->runningPlayer->gamemapPos.y()-senderPos.y());
            else
                distance = 999;
        }
        else
        {
            if(gameWindow->runningPlayer->gamemapPos.y() == senderPos.y())
                distance = fabs(gameWindow->runningPlayer->gamemapPos.y()-senderPos.y());
            else
                distance = 999;
        }

        if(distance > 1)
        {
            QGroupBox *msg = new QGroupBox(gameWindow->ui->gamePannel);
            QLabel *failLab = new QLabel(msg);
            failLab->setText("购买失败，距离太远");
            QPushButton *btn = new QPushButton(msg);
            btn->setText("确定");
            connect(btn,&QPushButton::clicked,[=](){
                msg->close();
                delete failLab;
                delete btn;
                delete msg;
            });

            failLab->move(0,0);
            btn->move(0,20);
            msg->move(gameWindow->ui->gamePannel->width()/2-msg->width()/2,gameWindow->ui->gamePannel->height()/2-msg->height()/2);
            msg->show();
        }
        else
        {
            //能够购买
            //获取金币
            Money *userMoney;
            for(int i = 0;i < gameWindow->runningPlayer->knapsack->container.length();i++)
            {
                if(gameWindow->runningPlayer->knapsack->container.at(i)->inherits("Money"))
                {
                    userMoney = dynamic_cast<Money*>(gameWindow->runningPlayer->knapsack->container.at(i));
                }
            }

            //购买成功
            if(userMoney->num > land->price)
            {
                userMoney->num -= land->price;

                emit userMoney->moneyChanged();

                //替换土地为房子
                AbstractMap *house = MapFactory::createMap(19,senderPos.x(),senderPos.y());
                house->setParent(gameWindow->ui->map);
                gameWindow->mapList[senderPos.x()].replace(senderPos.y(),house);
                emit mapChanged();


                //设置房子主人
                dynamic_cast<House*>(house)->owner = gameWindow->runningPlayer;
                //设置租金
                dynamic_cast<House*>(house)->rent = land->price;

                QGroupBox *msg = new QGroupBox(gameWindow->ui->gamePannel);
                QLabel *successLab = new QLabel(msg);
                successLab->setText("购买成功");
                QPushButton *btn = new QPushButton(msg);
                btn->setText("确定");
                connect(btn,&QPushButton::clicked,[=](){
                    msg->close();
                    delete successLab;
                    delete btn;
                    delete msg;
                });

                successLab->move(0,0);
                btn->move(0,20);
                msg->move(gameWindow->ui->gamePannel->width()/2 - msg->width()/2,gameWindow->ui->gamePannel->height()/2 - msg->height()/2);
                msg->show();
            }
            else
            //金币不足，购买失败
            {
                QGroupBox *msg = new QGroupBox(gameWindow->ui->gamePannel);
                QLabel *failLab = new QLabel(msg);
                failLab->setText("购买失败，金币不足");
                QPushButton *btn = new QPushButton(msg);
                btn->setText("确定");
                connect(btn,&QPushButton::clicked,[=](){
                    msg->close();
                    delete failLab;
                    delete btn;
                    delete msg;
                });

                failLab->move(0,0);
                btn->move(0,20);
                msg->move(gameWindow->ui->gamePannel->width()/2-msg->width()/2,gameWindow->ui->gamePannel->height()/2-msg->height()/2);
                msg->show();
            }

        }
    });

    buyPannel->show();
    return true;
}
