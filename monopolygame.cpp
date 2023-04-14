#include "monopolygame.h"
#include "ui_monopolygame.h"
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSize>
#include "house.h"
#include <QDebug>
#include <QTimer>
#include "mapfactory.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QShortcut>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include "Grass.h"
#include <QPushButton>
#include "road.h"
#include <QLabel>
#include "monopolygame.h"
#include "money.h"
#include <QFont>
#include <QtGlobal>
#include <QTime>
#include <QFile>
#include <QPainter>

monopolyGame* monopolyGame::myWindow = nullptr;

monopolyGame::monopolyGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monopolyGame)
{
    ui->setupUi(this);

    myWindow = this;

    this->setWindowTitle(QString("大富翁"));
    ui->gamePannel->setGeometry(0,0,1800,969);
    ui->map->setGeometry(0,0,1800,969);
    camearCenter = new QPoint(CAMERACENTER_X,CAMERACENTER_Y);

    QFile *qss = new QFile(":/res/qss/monopoly.qss");
    if(qss->open(QFile::ReadOnly))
    {
        setStyleSheet(qss->readAll());
    }



    initGameData();
    initGameMap();
    initUI();

    //绑定按键调整镜头
    QShortcut *shortcut_W = new QShortcut(QKeySequence(Qt::Key_W), this);
    shortcut_W->setObjectName("W");
    connect(shortcut_W, &QShortcut::activated, this, moveCamera);
    QShortcut *shortcut_S = new QShortcut(QKeySequence(Qt::Key_S), this);
    shortcut_S->setObjectName("S");
    connect(shortcut_S, &QShortcut::activated, this, moveCamera);
    QShortcut *shortcut_A = new QShortcut(QKeySequence(Qt::Key_A), this);
    shortcut_A->setObjectName("A");
    connect(shortcut_A, &QShortcut::activated, this, moveCamera);
    QShortcut *shortcut_D = new QShortcut(QKeySequence(Qt::Key_D), this);
    shortcut_D->setObjectName("D");
    connect(shortcut_D, &QShortcut::activated, this, moveCamera);

}

monopolyGame::~monopolyGame()
{
    delete ui;
}

bool monopolyGame::initGameMap()
{
    //初始化地图数组
    int gameMap[GAMEPANNEL_ROW][GAMEPANNEL_COL];
    for(int i = 0;i < GAMEPANNEL_ROW;i++)
    {
        for(int j = 0;j < GAMEPANNEL_COL;j++)
        {
            gameMap[i][j] = 1;
        }
    }


    //从map.txt中读取相应的指定地图元素
    QFile map("./map/map.txt");
    if (!map.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"打开失败";
        return false; // 如果无法打开文件，则退出
    }


    //更新地图数组
    QTextStream ins(&map);
    while(!ins.atEnd())
    {
        QStringList strList =  ins.readLine().split(" ");
        gameMap[strList[0].toInt()][strList[1].toInt()] = strList[2].toInt();
    }


    //生成地图
    mapList.clear();
    for(int i = 0;i < GAMEPANNEL_ROW;i++)
    {
        mapList.append(QList<AbstractMap*>());
        for(int j = 0;j < GAMEPANNEL_COL;j++)
        {
            AbstractMap* p = MapFactory::createMap(gameMap[i][j],i,j);
            p->setParent(ui->map);
            p->hide();
            mapList[i].append(p);
        }
    }

    printMap();

    map.close();
    return true;
}

bool monopolyGame::initGameData()
{
    QPoint begin;

    QFile map("./map/map.txt");
    if (!map.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false; // 如果无法打开文件，则退出
    }
    QTextStream ins(&map);
    if(ins.atEnd())
    {
        QMessageBox::information(this,"游戏文件无效","初始化游戏数据player数据时遇到了无效数据");
        exit(0);
    }
    else
    {
        QStringList strList =  ins.readLine().split(" ");
        if(strList[2].toInt() == 9)
        {
            begin.setX(strList[0].toInt());
            begin.setY(strList[1].toInt());
        }
    }

    //生成玩家
    for(int playerNum = 0; playerNum < PLAYER_NUM;playerNum++)
    {
        playerList.append(new Player(begin));
        QLabel *lab = new QLabel();
        lab->setText(playerList.at(playerNum)->name);
        playerTitleList.append(lab);

        Money *userMoney;
        for(int propNum = 0;propNum < playerList[playerNum]->knapsack->container.length();propNum++)
        {
            if(playerList[playerNum]->knapsack->container.at(propNum)->inherits("Money"))
            {
                userMoney = dynamic_cast<Money*>(playerList[playerNum]->knapsack->container.at(propNum));
                break;
            }
        }

        //绑定金币改变事件
        connect(userMoney,&Money::moneyChanged,[=](){
            Money *runningUserMoney;
            for(int propNum = 0;propNum < runningPlayer->knapsack->container.length();propNum++)
            {
                if(runningPlayer->knapsack->container.at(propNum)->inherits("Money"))
                {
                    runningUserMoney = dynamic_cast<Money*>(runningPlayer->knapsack->container.at(propNum));
                }
            }
            ui->gamePannel->findChild<QLabel*>("runningPlayer_moneyLab")->setText("余额：" + QString::number(runningUserMoney->num));
        });


        //绑定行走完毕事件
        connect(playerList[playerNum],&Player::playerRun,[=](){

            //扣除租金
            Road *road = dynamic_cast<Road*>(mapList[runningPlayer->gamemapPos.x()][runningPlayer->gamemapPos.y()]);
            //找到这个道路对应的两个可能的房子
            QList<House*> houseList;
            House* house;
            switch (road->direction) {
            case Road::Direct::LEFT:
            case Road::Direct::RIGHT:
                if(mapList[runningPlayer->gamemapPos.x()-1][runningPlayer->gamemapPos.y()]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->gamemapPos.x()-1][runningPlayer->gamemapPos.y()]);
                    if(house->owner != runningPlayer)houseList.append(house);
                }
                if(mapList[runningPlayer->gamemapPos.x()+1][runningPlayer->gamemapPos.y()]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->gamemapPos.x()+1][runningPlayer->gamemapPos.y()]);
                    if(house->owner != runningPlayer)houseList.append(house);
                }
                break;
            case Road::Direct::UP:
            case Road::Direct::DOWN:
                if(mapList[runningPlayer->gamemapPos.x()][runningPlayer->gamemapPos.y()-1]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->gamemapPos.x()][runningPlayer->gamemapPos.y()-1]);
                    if(house->owner != runningPlayer)houseList.append(house);
                }
                if(mapList[runningPlayer->gamemapPos.x()][runningPlayer->gamemapPos.y()+1]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->gamemapPos.x()][runningPlayer->gamemapPos.y()+1]);
                    if(house->owner != runningPlayer)houseList.append(house);
                }
                break;
            }


            //依次显示扣除租金提示
            for(int i = 0;i < houseList.length();i++)
            {
                QGroupBox *msg = new QGroupBox(ui->gamePannel);
                int width=200,height=100;
                msg->setGeometry(ui->gamePannel->width()/2-width/2,ui->gamePannel->height()/2-height/2,width,height);
                QLabel *lab = new QLabel(msg);
                QPushButton *confirmBtn = new QPushButton(msg);

                lab->setStyleSheet("font-size:20px;");

                lab->move(50,0);
                confirmBtn->move(75,75);

                lab->setText("扣除租金："+QString::number(houseList[i]->rent));
                confirmBtn->setText("确认");

                connect(confirmBtn,&QPushButton::clicked,[=](){
                    msg->close();
                    delete lab;
                    delete confirmBtn;
                    delete msg;
                });

                msg->show();

                //获取金币指针
                Money *runningPlayerMoney;
                for(int j = 0;j < runningPlayer->knapsack->container.length();j++)
                {
                    if(runningPlayer->knapsack->container.at(j)->inherits("Money"))
                    {
                        runningPlayerMoney = dynamic_cast<Money*>(runningPlayer->knapsack->container.at(j));
                    }
                }

                runningPlayerMoney->num -= houseList[i]->rent;
                emit runningPlayerMoney->moneyChanged();
            }
            showPlayer();
        });


        //绑定行走玩家发生变化事件
        connect(playerList[playerNum],&Player::playerChanged,[=](){

            QLabel* lab = ui->gamePannel->findChild<QLabel*>("runningPlayerLab");
            lab->setText(runningPlayer->name);

            Money *runningUserMoney;
            for(int j = 0;j < runningPlayer->knapsack->container.length();j++)
            {
                if(runningPlayer->knapsack->container.at(j)->inherits("Money"))
                {
                    runningUserMoney = dynamic_cast<Money*>(runningPlayer->knapsack->container.at(j));
                }
            }
            ui->gamePannel->findChild<QLabel*>("runningPlayer_moneyLab")->setText("余额：" + QString::number(runningUserMoney->num));
        });
    }



    runningPlayer = playerList[0];

    return true;
}


bool monopolyGame::initUI()
{
    //走一步按钮
    QPushButton *run = new QPushButton(ui->gamePannel);
    run->setObjectName("run");
    run->setText("走一次");
    run->move(ui->gamePannel->width()-250,ui->gamePannel->height()-100);
    run->show();
    connect(run,&QPushButton::clicked,this,&playerRun);

    //回合结束按钮
    QPushButton *endOfTurn = new QPushButton(ui->gamePannel);
    endOfTurn->setObjectName("nextPlayer");
    endOfTurn->setText("回合结束");
    endOfTurn->move(ui->gamePannel->width()-150,ui->gamePannel->height()-100);
    endOfTurn->show();
    connect(endOfTurn,&QPushButton::clicked,this,&monopolyGame::endOfTurn);


    //当前玩家信息标签(groupBox)
    QGroupBox *runningPlayerInfo = new QGroupBox(ui->gamePannel);
    runningPlayerInfo->setObjectName("runningPlayerInfo");
    runningPlayerInfo->setGeometry(10,10,300,200);


    //当前玩家label
    QLabel *runningPlayerLab = new QLabel(runningPlayerInfo);
    runningPlayerLab->setObjectName("runningPlayerLab");
    runningPlayerLab->setText(runningPlayer->name);
    runningPlayerLab->move(0,0);
    runningPlayerLab->show();

    //余额label
    QLabel *runningPlayer_moneyLab = new QLabel(runningPlayerInfo);
    runningPlayer_moneyLab->setObjectName("runningPlayer_moneyLab");
    for(int i = 0;i < runningPlayer->knapsack->container.length();i++)
    {
        if(runningPlayer->knapsack->container.at(i)->inherits("Money"))
        {
            runningPlayer_moneyLab->setText("余额：" +QString::number(dynamic_cast<Money*>(runningPlayer->knapsack->container.at(i))->num) );
            break;
        }
    }
    runningPlayer_moneyLab->move(0,100);
    runningPlayer_moneyLab->show();

    //背包按钮

    //信息按钮
    QPushButton *info = new QPushButton(ui->gamePannel);
    info->setObjectName("infoBtn");
    info->setText("?");
    info->move(ui->gamePannel->width()-60,0);
    connect(info,&QPushButton::clicked,[=](){
        if(ui->gamePannel->findChild<QGroupBox*>("infoBox"))return;

        QGroupBox *infoBox = new QGroupBox(ui->gamePannel);
        int width = 300,height = 300;
        infoBox->setGeometry(ui->gamePannel->width()/2 - width/2,ui->gamePannel->height()/2 - height/2,width,height);
        infoBox->setObjectName("infoBox");

        QLabel *infoLab = new QLabel(infoBox);
        infoLab->move(0,0);
        infoLab->setText("移动镜头：WASD分别对应上左下右");

        QPushButton *confirm = new QPushButton(infoBox);
        confirm->setText("确认");
        confirm->move(infoBox->width()/2 - confirm->width()/2,infoBox->height() - confirm->height());
        connect(confirm,&QPushButton::clicked,[=](){
            infoBox->close();
            delete infoLab;
            delete confirm;
            delete infoBox;
        });

        infoBox->show();
    });
    info->show();

    return true;
}

bool monopolyGame::printMap()
{
    //打印地图
    bool showPlayerFlag = false;
    for(int i = camearCenter->y()-SCREEN_H_ELEMS/2,girdI = 0 ; i < camearCenter->y()+SCREEN_H_ELEMS/2;i++,girdI++)
    {
        for(int j = camearCenter->x() - SCREEN_W_ELEMS/2 ,girdJ = 0; j < camearCenter->x() + SCREEN_W_ELEMS/2;j++,girdJ++)
        {
            AbstractMap* p = mapList[i][j];
            p->move(girdJ*ELEM_W,girdI*ELEM_H);
            p->show();
        }
    }

    //渲染玩家
    showPlayerFlag = showPlayer();

    //打印玩家
    //结束后还没有渲染玩家，说明玩家应该暂时消失
    if(!showPlayerFlag)
    {
        for(int num = 0;num < this->playerList.length();num++)
        {
            this->playerList[num]->hide();
            this->playerTitleList[num]->hide();
        }
    }

    return true;
}


bool monopolyGame::showPlayer()
{
    bool showPlayerFlag = false;
    for(int i = camearCenter->y()-SCREEN_H_ELEMS/2,girdI = 0 ; i < camearCenter->y()+SCREEN_H_ELEMS/2;i++,girdI++)
    {
        for(int j = camearCenter->x() - SCREEN_W_ELEMS/2 ,girdJ = 0; j < camearCenter->x() + SCREEN_W_ELEMS/2;j++,girdJ++)
        {

            for(int num = 0;num < this->playerList.length();num++)
            {
                if(this->playerList[num]->gamemapPos.x() == i && this->playerList[num]->gamemapPos.y() == j)
                {
                    //玩家图片
                    this->playerList[num]->setParent(ui->gamePannel);
                    this->playerList[num]->move(girdJ*36,girdI*51);
                    this->playerList[num]->show();

                    //玩家头衔
                    this->playerTitleList[num]->setParent(ui->gamePannel);
                    this->playerTitleList[num]->move(girdJ*36,girdI*51 -15);
                    this->playerTitleList[num]->show();

                    showPlayerFlag = true;
                }
            }
        }
    }

    return showPlayerFlag;
}

Player *monopolyGame::nextPlayer()
{
    int index = playerList.indexOf(runningPlayer);
    if(index+1 >= PLAYER_NUM)
    {
        return playerList[0];
    }
    else
    {
        return playerList[index+1];
    }
}


bool monopolyGame::moveCamera()
{
    QObject* sender = QObject::sender();
    QString signalName = sender->objectName();


    QPoint point(camearCenter->x(),camearCenter->y());
    switch (signalName.at(0).unicode()) {
    case 'W':
        camearCenter->setY(camearCenter->y()-1<SCREEN_H_ELEMS/2?SCREEN_H_ELEMS/2:camearCenter->y()-1);
        break;
    case 'A':
        camearCenter->setX(camearCenter->x()-1>SCREEN_W_ELEMS/2?camearCenter->x()-1:SCREEN_W_ELEMS/2);
        break;
    case 'S':
        camearCenter->setY(camearCenter->y()+1>GAMEPANNEL_ROW-SCREEN_H_ELEMS/2?GAMEPANNEL_ROW-SCREEN_H_ELEMS/2:camearCenter->y()+1);
        break;
    case 'D':
        camearCenter->setX(camearCenter->x()+1<GAMEPANNEL_COL-SCREEN_W_ELEMS/2?camearCenter->x()+1:GAMEPANNEL_COL-SCREEN_W_ELEMS/2);
        break;
    default:
        break;
    }

    if(point.x()!=camearCenter->x() || point.y()!=camearCenter->y())
    {
        //清空原屏幕layout
        for(int i = point.y()-SCREEN_H_ELEMS/2; i < point.y()+SCREEN_H_ELEMS/2;i++)
        {
            for(int j = point.x() - SCREEN_W_ELEMS/2 ; j < point.x() + SCREEN_W_ELEMS/2;j++)
            {
                //mapList[i][j]->setParent(nullptr);频繁的设置会占用大量cpu资源
                mapList[i][j]->hide();
            }
        }
        ui->map->show();

        printMap();
    }
    return true;
}

bool monopolyGame::playerRun()
{

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    runningPlayer->steps = qrand()%6+1;

    ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(false);
    int x = runningPlayer->gamemapPos.x();
    int y = runningPlayer->gamemapPos.y();
    Road *road = dynamic_cast<Road*>(mapList[x][y]);

    while(runningPlayer->steps != 0 && runningPlayer->steps >= road->stepCost)
    {
        switch (road->direction) {
        case Road::Direct::RIGHT:
            runningPlayer->gamemapPos.setY(y+1);
            break;
        case Road::Direct::LEFT:
            runningPlayer->gamemapPos.setY(y-1);
            break;
        case Road::Direct::UP:
            runningPlayer->gamemapPos.setX(x-1);
            break;
        case Road::Direct::DOWN:
            runningPlayer->gamemapPos.setX(x+1);
            break;
        default:
            break;
        }
        runningPlayer->steps -=road->stepCost;

        x = runningPlayer->gamemapPos.x();
        y = runningPlayer->gamemapPos.y();
        road = dynamic_cast<Road*>(mapList[x][y]);
    }

    runningPlayer->steps = 0;

    //发出玩家行走完毕信号
    emit runningPlayer->playerRun();
    return true;
}


bool monopolyGame::endOfTurn()
{
    ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(true);
    runningPlayer = nextPlayer();
    emit runningPlayer->playerChanged();
    return true;
}
