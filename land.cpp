#include "land.h"
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>
#include <QtGlobal>
#include "money.h"
#include <QMessageBox>
#include "house.h"
#include <QRandomGenerator>


Land::Land(int x,int y):OperablePlace(x,y)
{
    setPixmap(QPixmap(":/res/img/land.png"));
    QObject::connect(this,SIGNAL(clicked()),this,SLOT(showBuyHouseUI()));
    QObject::connect(this,&Land::mapChanged,[=](){
        gameWindow->printMap();
    });

    price = QRandomGenerator::global()->bounded(MIN_PRICE,MAX_PRICE);
}

float Land::getPrice() const
{
    return price;
}

void Land::setPrice(float newPrice)
{
    price = newPrice;
}

bool Land::showBuyHouseUI()
{
    //防止重复出现，导致游戏逻辑出现问题
    if(gameWindow->ui->gamePannel->findChild<QGroupBox*>("landBuyPannel") != nullptr)return false;

    QPoint senderPos = dynamic_cast<Land*>(sender())->gamemapPos;
    Land *land = dynamic_cast<Land*>(gameWindow->mapList[senderPos.x()][senderPos.y()]);

    int buyPannel_width = 300,buyPannel_height = 200;
    QGroupBox *buyPannel = new QGroupBox(gameWindow->ui->gamePannel);
    buyPannel->setGeometry(gameWindow->ui->gamePannel->width()/2-buyPannel_width/2,gameWindow->ui->gamePannel->height()/2-buyPannel_height/2,buyPannel_width,buyPannel_height);
    buyPannel->setObjectName("landBuyPannel");

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
        delete buyPannel;
    });

    //确认购买
    connect(confirm,&QPushButton::clicked,[=](){
        buyPannel->close();
        delete buyPannel;


        //获取距离
        double distance;
        QPoint landPos = this->gamemapPos;
        QPoint playerPos = gameWindow->runningPlayer->getGamemapPos();
        distance = sqrt(pow(landPos.x()-playerPos.x(),2) + pow(landPos.y()-playerPos.y(),2));

        //如果距离太远
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
            Money *userMoney = dynamic_cast<Money*>(gameWindow->runningPlayer->getKnapsack()->getProp("Money"));

            //购买成功
            if(userMoney->getNum() > land->price)
            {
                userMoney->reduce(land->price);

                emit userMoney->moneyChanged();

                //替换土地为房子
                House *house = dynamic_cast<House*>(MapFactory::createMap(19,senderPos.x(),senderPos.y(),gameWindow->runningPlayer));
                house->setParent(gameWindow->ui->map);
                if(gameWindow->isAroundClass(house->gamemapPos,"Black"))
                {
                    house->setRent(house->getRent()/2);
                }
                gameWindow->mapList[senderPos.x()].replace(senderPos.y(),house);
                emit mapChanged();


                //显示购买成功
                QGroupBox *msg = new QGroupBox(gameWindow->ui->gamePannel);
                QLabel *successLab = new QLabel(msg);
                successLab->setText("购买成功");
                QPushButton *btn = new QPushButton(msg);
                btn->setText("确定");
                connect(btn,&QPushButton::clicked,[=](){
                    msg->close();
                    delete msg;
                });

                successLab->move(40,10);
                btn->move(30,60);
                msg->setGeometry(gameWindow->ui->gamePannel->width()/2 - 75,gameWindow->ui->gamePannel->height()/2 - 50,150,100);
                msg->setStyleSheet("QLabel{font-size:15px;}");
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
