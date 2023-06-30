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
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <sleepy.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include "sleepycard.h"
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProgressBar>
#include <QThread>
#include <QOverload>
//#include <QRandomGenerator>

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
    ui->mainUI->setGeometry(0,0,1800,969);
    ui->prepareUI->setGeometry(0,0,1800,969);

    ui->mainUI->show();
    ui->prepareUI->hide();
    ui->gamePannel->hide();

    //应用QSS
    QFile *qss = new QFile(":/res/qss/monopoly.qss");
    if(qss->open(QFile::ReadOnly))
    {
        setStyleSheet(qss->readAll());
    }
    qss->close();

    //初始化游戏配置
    if(!initSettings())exit(-1);

    showMainUI();

    connect(this,&monopolyGame::beginInit,this,[=](){
        ui->mainUI->hide();
        ui->prepareUI->show();
        initGame();
        stateController.setState(gameState::Running);

        //test area
        runningPlayer->knapsack->addProp(new SleepCard());


        ui->prepareUI->hide();
        ui->gamePannel->show();
    });

    connect(this,&monopolyGame::initStepAdd,this,[=](){
        QProgressBar *progressBar = ui->prepareUI->findChild<QProgressBar*>("initBar");
        int value = progressBar->value();
        if(value+1 <= progressBar->maximum())
            progressBar->setValue(value+1);
    });

}

monopolyGame::~monopolyGame()
{
    delete ui;
}

bool monopolyGame::showMainUI()
{
    initPerpareUI();
    initMainUI();

    ui->mainUI->show();
    return true;
}

bool monopolyGame::initMainUI()
{
    //开始游戏按钮
    QPushButton *startGameBtn = new QPushButton(ui->mainUI);
    startGameBtn->setGeometry(0,0,150,50);
    startGameBtn->move(ui->mainUI->width()/2-startGameBtn->width()/2,ui->mainUI->height()*0.75);
    startGameBtn->setText("开始游戏");
    QObject::connect(startGameBtn,&QPushButton::clicked,[=](){
        ui->mainUI->hide();
        emit beginInit();
    });

    //标题图标
    QLabel *title = new QLabel(ui->mainUI);
    QPixmap titleImg(":/res/img/title.png");
    titleImg = titleImg.scaledToWidth(800);
    title->setPixmap(titleImg);
    title->move(ui->mainUI->width()/2-titleImg.width()/2,20);

    //游戏设置按钮
    return true;
}

bool monopolyGame::initPerpareUI()
{
    int initBarHeight = 20,initLabHeight = 50,initLabWidth = 200;
    //进度条
    QProgressBar *initBar = new QProgressBar(ui->prepareUI);
    initBar->setObjectName("initBar");
    initBar->setRange(0,4);
    initBar->setValue(0);
    initBar->setGeometry(0,0,ui->prepareUI->width(),initBarHeight);
    initBar->move(0,ui->prepareUI->height()-initBarHeight-initLabHeight);
    //说明文本
    QLabel *initLab = new QLabel(ui->prepareUI);
    initLab->setObjectName("initLab");
    initLab->setGeometry(ui->prepareUI->width()/2-initLabWidth/2,ui->prepareUI->height()-initLabHeight,initLabWidth,initLabHeight);
    initLab->setAlignment(Qt::AlignCenter);


    //背景图

    return true;
}

bool monopolyGame::initGame()
{
    QLabel *lab = ui->prepareUI->findChild<QLabel*>("initLab");

    lab->setText("正在初始化游戏数据");
    if(!initGameData())return false;
    emit initStepAdd();
    lab->setText("正在初始化游戏地图");
    if(!initGameMap())return false;
    emit initStepAdd();
    lab->setText("正在初始化游戏UI");
    if(!initUI())return false;
    emit initStepAdd();
    lab->setText("正在初始化游戏逻辑");
    checkMapLogic();
    emit initStepAdd();
    return true;
}

bool monopolyGame::initSettings()
{
    //获取json文件的信息，生成json对象
    QFile *settingsFile = new QFile("./settings/settings.json");
    if(!settingsFile->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::information(this,"游戏文件错误","核心配置文件不存在！");
        return false;
    }
    QJsonParseError jerr;
    QJsonDocument settingsDoc = QJsonDocument::fromJson(settingsFile->readAll(),&jerr);
    if(settingsDoc.isNull())
    {
        QMessageBox::information(this,"游戏文件错误","核心配置文件格式错误\n！" + jerr.errorString());
        return false;
    }
    QJsonObject settingsObj = settingsDoc.object();


    //利用json对象初始化游戏数据
    ElemW = settingsObj["ElemW"].toInt();
    ElemH = settingsObj["ElemH"].toInt();
    ScreenWElems = settingsObj["ScreenWElems"].toInt();
    ScreenHElems = settingsObj["ScreenHElems"].toInt();
    GamepannelRow = settingsObj["GamepannelRow"].toInt();
    GamepannelCol = settingsObj["GamepannelCol"].toInt();
    CameracenterX = settingsObj["CameracenterX"].toInt();
    CameracenterY = settingsObj["CameracenterY"].toInt();
    PlayerNum = settingsObj["PlayerNum"].toInt();
    initPlayerMoneyNum = settingsObj["initPlayerMoneyNum"].toDouble();
    playerNameList = settingsObj["playerNameList"].toArray().toVariantList();
    if(playerNameList.count() != PlayerNum)
    {
        QMessageBox::information(this,"游戏文件错误","游戏玩家信息不匹配：玩家数量和玩家名字数量不一致\n！");
        return false;
    }

    settingsFile->close();

    //默认开启游戏声音
    musicThread = new MusicThread();
    musicThread->start();

    return true;
}

bool monopolyGame::initGameMap()
{
    //初始化地图数组
    int gameMap[GamepannelRow][GamepannelCol];
    for(int i = 0;i < GamepannelRow;i++)
    {
        for(int j = 0;j < GamepannelCol;j++)
        {
            gameMap[i][j] = 1;
        }
    }


    //从map.txt中读取相应的指定地图元素
    QFile map("./map/map.txt");
    if (!map.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this,"游戏文件错误","似乎没有找到相应的地图文件。");
        qDebug()<<"打开失败";
        return false; // 如果无法打开文件，则退出
    }


    //更新地图数组
    QTextStream ins(&map);
    while(!ins.atEnd())
    {
        QStringList strList =  ins.readLine().split(" ");
        //过滤无效数据和注释数据
        if(strList.length() == 3)
        {
            gameMap[strList[0].toInt()][strList[1].toInt()] = strList[2].toInt();
        }
    }


    //生成地图
    mapList.clear();
    for(int i = 0;i < GamepannelRow;i++)
    {
        mapList.append(QList<AbstractMap*>());
        for(int j = 0;j < GamepannelCol;j++)
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
    //设置相机中心
    camearCenter = new QPoint(CameracenterX,CameracenterY);

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
        exit(-1);
    }
    else
    {
        QStringList strList =  ins.readLine().split(" ");
        while(strList.length() != 3)
            strList =  ins.readLine().split(" ");
        if(strList[2].toInt() == 9)
        {
            begin.setX(strList[0].toInt());
            begin.setY(strList[1].toInt());
        }
        else
        {
            QMessageBox::information(this,"地图加载失败","地图起点加载失败，请检查地图格式。");
            exit(-1);
        }
    }

    //生成玩家
    for(int num = 0; num < PlayerNum;num++)
    {
        playerList.append(new Player(begin,initPlayerMoneyNum));
        playerList.at(num)->name = playerNameList.at(num).toString();
        playerList.at(num)->setParent(ui->gamePannel);

        //生成头衔
        QLabel *lab = new QLabel(ui->gamePannel);
        lab->setText(playerList.at(num)->name);
        lab->setGeometry(0,0,100,30);
        lab->setStyleSheet("font-size:20px;");
        playerTitleList.append(lab);

        Money *userMoney = dynamic_cast<Money*>(playerList[num]->knapsack->getProp("Money"));

        //绑定金币改变事件
        connect(userMoney,&Money::moneyChanged,[=](){
            Money *runningUserMoney = dynamic_cast<Money*>(runningPlayer->knapsack->getProp("Money"));
            ui->gamePannel->findChild<QLabel*>("runningPlayer_moneyLab")->setText("余额：" + QString::number(runningUserMoney->getNum()));

            //更新玩家控制器
            if(runningUserMoney->getNum() < 0)runningPlayer->stateController.setState(PlayerState::Bankruptcy);
        });


        //绑定行走完毕事件
        connect(playerList[num],&Player::playerRun,[=](){

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
                Money *runningPlayerMoney = dynamic_cast<Money*>(runningPlayer->knapsack->getProp("Money"));
                runningPlayerMoney->reduce(houseList[i]->rent);
                dynamic_cast<Money*>(houseList[i]->owner->knapsack->getProp("Money"))->add(houseList[i]->rent);
                emit runningPlayerMoney->moneyChanged();
            }
        });


        //绑定行走玩家发生变化事件
        connect(playerList[num],&Player::playerChanged,[=](){

            QLabel* lab = ui->gamePannel->findChild<QLabel*>("runningPlayerLab");
            lab->setText(runningPlayer->name);

            Money *runningUserMoney = dynamic_cast<Money*>(runningPlayer->knapsack->getProp("Money"));
            ui->gamePannel->findChild<QLabel*>("runningPlayer_moneyLab")->setText("余额：" + QString::number(runningUserMoney->getNum()));
        });
    }



    runningPlayer = playerList[0];

    return true;
}

bool monopolyGame::checkMapLogic()
{
    for(int i = 0;i < mapList.length();i++)
    {
        QList<AbstractMap*> list = mapList.at(i);
        for(int j = 0;j < list.length();j++)
        {
            if(dynamic_cast<Road*>(list.at(j))!=nullptr && isAroundClass(list.at(j)->gamemapPos,"BlackHole"))
            {
               Road* p = dynamic_cast<Road*>(list.at(j));
               p->setStepCost(2);
            }
        }
    }
    return true;
}


bool monopolyGame::initUI()
{
    //获取UIjson文件
    QFile UIFile("./settings/UI.json");
    if(!UIFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::information(this,"游戏文件错误","打开UI配置文件失败。");
        exit(-1);
    }
    QJsonParseError jerr;
    QJsonDocument UIDoc = QJsonDocument::fromJson(UIFile.readAll(),&jerr);
    if(UIDoc.isNull())
    {
        QMessageBox::information(this,"游戏文件错误","UI配置文件格式错误。\n"+jerr.errorString());
        exit(-1);
    }

    QJsonObject UIJson = UIDoc.object();


    //走一步按钮
    QPushButton *run = new QPushButton(ui->gamePannel);
    run->setObjectName("run");
    run->setText("走一次");
    run->move(ui->gamePannel->width()-250,ui->gamePannel->height()-100);
    run->show();
    connect(run,&QPushButton::clicked,this,QOverload<>::of(&playerRun));

    //回合结束按钮
    QPushButton *endOfTurn = new QPushButton(ui->gamePannel);
    endOfTurn->setObjectName("nextPlayer");
    endOfTurn->setText("回合结束");
    endOfTurn->move(ui->gamePannel->width()-150,ui->gamePannel->height()-100);
    endOfTurn->show();
    connect(endOfTurn,&QPushButton::clicked,this,&monopolyGame::endOfTurn);

    //背包按钮
    QPushButton *knapsackBtn = new QPushButton(ui->gamePannel);
    QPixmap knapsackIco(":/res/img/knapsack.ico");
    knapsackBtn->setObjectName("knapsackBtn");
    knapsackBtn->setCursor(Qt::PointingHandCursor);
    knapsackBtn->setIcon(knapsackIco);
    knapsackBtn->setIconSize(knapsackIco.size());
    knapsackBtn->setGeometry(ui->gamePannel->width() - knapsackIco.width() -10,50,knapsackIco.width(),knapsackIco.height());
    connect(knapsackBtn,&QPushButton::clicked,[=](){
        QGroupBox *knapsackUI = ui->gamePannel->findChild<QGroupBox*>("knapsackUI");
        if(knapsackUI == nullptr)
        {
            //创建总的UI
            knapsackUI = new QGroupBox(ui->gamePannel);
            knapsackUI->setGeometry(ui->gamePannel->width()/2 - 750,50,1500,800);
            knapsackUI->setObjectName("knapsackUI");


            //UI头部元素
            QGroupBox* headBox = new QGroupBox(knapsackUI);
            QHBoxLayout *headLayout = new QHBoxLayout();
            headBox->setGeometry(0,0,knapsackUI->width(),knapsackUI->height()*0.2);//设置区域大小
            headBox->setObjectName("headBox");
            //关闭按钮
            QPushButton *knapsackUI_closeBtn = new QPushButton(headBox);
            knapsackUI_closeBtn->setObjectName("knapsackUI_closeBtn");
            knapsackUI_closeBtn->setCursor(Qt::PointingHandCursor);
            knapsackUI_closeBtn->setText("X");
            connect(knapsackUI_closeBtn,&QPushButton::clicked,[=](){
                knapsackUI->hide();
            });
            //背包信息
            QLabel *knapsackUI_knapsackInfo = new QLabel(headBox);
            knapsackUI_knapsackInfo->setText(runningPlayer->name+"的背包");
            knapsackUI_knapsackInfo->setObjectName("knapsackUI_knapsackInfo");

            headLayout->addWidget(knapsackUI_knapsackInfo);
            headLayout->setAlignment(knapsackUI_knapsackInfo,Qt::AlignLeft|Qt::AlignTop);
            headLayout->addWidget(knapsackUI_closeBtn);
            headLayout->setAlignment(knapsackUI_closeBtn,Qt::AlignRight|Qt::AlignTop);
            headBox->setLayout(headLayout);

            //UIbody元素
            QGroupBox *bodyBox = new QGroupBox(knapsackUI);
            bodyBox->setObjectName("bodyBox");
            bodyBox->setGeometry(0,headBox->height(),knapsackUI->width(),knapsackUI->height()*0.8);
            //物品列表
            QScrollArea *propList = new QScrollArea(bodyBox);
            propList->setObjectName("propList");
            propList->setGeometry(0,0,bodyBox->width(),bodyBox->height());
            propList->setWidget(runningPlayer->knapsack->getTab());

            knapsackUI->show();
        }
        else
        {
            //更新背包信息为当前玩家的背包
            QGroupBox* headUI = ui->gamePannel->findChild<QGroupBox*>("knapsackUI")->findChild<QGroupBox*>("headBox");
            QGroupBox* bodyUI = ui->gamePannel->findChild<QGroupBox*>("knapsackUI")->findChild<QGroupBox*>("bodyBox");

            headUI->findChild<QLabel*>("knapsackUI_knapsackInfo")->setText(runningPlayer->name+"的背包");
            bodyUI->findChild<QScrollArea*>("propList")->setWidget(runningPlayer->knapsack->getTab());
            if(knapsackUI->isHidden())
                knapsackUI->show();
        }
    });

    knapsackBtn->show();



    //当前玩家信息
    QGroupBox *runningPlayerInfo = new QGroupBox(ui->gamePannel);
    runningPlayerInfo->setObjectName("runningPlayerInfo");
    runningPlayerInfo->setGeometry(10,10,300,200);
    //当前玩家label
    QLabel *runningPlayerLab = new QLabel(runningPlayerInfo);
    runningPlayerLab->setObjectName("runningPlayerLab");
    runningPlayerLab->setText(runningPlayer->name);
    runningPlayerLab->setGeometry(0,0,200,50);
    runningPlayerLab->show();
    //余额label
    QLabel *runningPlayer_moneyLab = new QLabel(runningPlayerInfo);
    runningPlayer_moneyLab->setObjectName("runningPlayer_moneyLab");
    runningPlayer_moneyLab->setText("余额：" +QString::number(dynamic_cast<Money*>(runningPlayer->knapsack->getProp("Money"))->getNum()));
    runningPlayer_moneyLab->move(0,100);
    runningPlayer_moneyLab->show();

    //回合信息标签
    QLabel* roundLab = new QLabel(ui->gamePannel);
    roundLab->setObjectName("currentRoundLab");
    roundLab->setText("第" + QString::number(roundController.getRound()) + "回合");
    roundLab->setGeometry(ui->gamePannel->width()/2-50,10,100,30);
    roundLab->show();
    //绑定刷新信息槽函数
    connect(&roundController,&GameRoundController::valueChanged,[=](){
        roundLab->setText("第" + QString::number(roundController.getRound()) + "回合");
        for(int i = 0;i < playerList.length();i++)
        {
            playerList.at(i)->stateController.refreshAffect(roundController.getRound());
        }
    });
    runningPlayerInfo->show();


    //游戏信息按钮
    QPushButton *info = new QPushButton(ui->gamePannel);
    info->setObjectName("infoBtn");
    info->setText("?");
    info->move(ui->gamePannel->width()-60,0);
    connect(info,&QPushButton::clicked,[=](){
        if(ui->gamePannel->findChild<QGroupBox*>("infoBox"))return;

        QGroupBox *infoBox = new QGroupBox(ui->gamePannel);
        infoBox->setObjectName("infoBox");
        int width = 500,height = 500;
        infoBox->setGeometry(ui->gamePannel->width()/2 - width/2,ui->gamePannel->height()/2 - height/2,width,height);
        infoBox->setObjectName("infoBox");

        QLabel *infoLab = new QLabel(infoBox);
        infoLab->setGeometry(0,0,width,height*0.8);
        infoLab->setText(UIJson["infoLabText"].toString());
        infoLab->setWordWrap(true);

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
    //显示玩家和头衔
    for(int num = 0;num < PlayerNum;num++)
    {
        playerList.at(num)->show();
        playerTitleList.at(num)->show();
    }
    //打印地图
    bool showPlayerFlag = false;
    for(int i = camearCenter->y()-ScreenHElems/2,girdI = 0 ; i < camearCenter->y()+ScreenHElems/2;i++,girdI++)
    {
        for(int j = camearCenter->x() - ScreenWElems/2 ,girdJ = 0; j < camearCenter->x() + ScreenWElems/2;j++,girdJ++)
        {
            AbstractMap* p = mapList[i][j];
            p->move(girdJ*ElemW,girdI*ElemH);
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

bool monopolyGame::checkState()
{
    int bankruptcyNum = 0;
    for(int num = 0; num < playerList.length();num++)
    {
        if(playerList[num]->stateController.getState() == PlayerState::Bankruptcy)
        {
            bankruptcyNum++;
        }
    }
    if(bankruptcyNum == PlayerNum-1)
        stateController.setState(gameState::End);
    return true;
}


bool monopolyGame::showPlayer()
{
    bool showPlayerFlag = false;
    //遍历屏幕寻找玩家坐标
    for(int i = camearCenter->y()-ScreenHElems/2,girdI = 0 ; i < camearCenter->y()+ScreenHElems/2;i++,girdI++)
    {
        for(int j = camearCenter->x() - ScreenWElems/2 ,girdJ = 0; j < camearCenter->x() + ScreenWElems/2;j++,girdJ++)
        {

            for(int num = 0;num < this->playerList.length();num++)
            {
                if(this->playerList[num]->gamemapPos.x() == i && this->playerList[num]->gamemapPos.y() == j)
                {
                    //玩家图片
                    this->playerList[num]->setParent(ui->gamePannel);
                    //玩家动画
                    QPropertyAnimation* playerAnimation = new QPropertyAnimation(playerList[num],"geometry");
                    playerAnimation->setDuration(800);//动画时间
                    playerAnimation->setStartValue(playerList[num]->geometry());
                    playerAnimation->setEndValue(QRect(girdJ*36,girdI*51,playerList[num]->geometry().width(),playerList[num]->geometry().height()));
                    playerAnimation->setEasingCurve(QEasingCurve::Linear);
                    playerAnimation->start(QAbstractAnimation::DeleteWhenStopped);


                    //玩家头衔
                    this->playerTitleList[num]->setParent(ui->gamePannel);
                    //头衔动画
                    QPropertyAnimation* playerTitleAnimation = new QPropertyAnimation(playerTitleList[num],"geometry");
                    playerTitleAnimation->setDuration(800);//动画时间
                    playerTitleAnimation->setStartValue(playerTitleList[num]->geometry());
                    playerTitleAnimation->setEndValue(QRect(girdJ*36,girdI*51-21,playerTitleList[num]->geometry().width(),playerTitleList[num]->geometry().height()));
                    playerTitleAnimation->setEasingCurve(QEasingCurve::Linear);
                    playerTitleAnimation->start(QAbstractAnimation::DeleteWhenStopped);

                    showPlayerFlag = true;
                }
            }
        }
    }


    return showPlayerFlag;
}

bool monopolyGame::isAroundClass(QPoint mapPoint,QString className)
{
    QPoint points[8] = {
        QPoint(mapPoint.x()-1,mapPoint.y()-1),
        QPoint(mapPoint.x()-1,mapPoint.y()),
        QPoint(mapPoint.x()-1,mapPoint.y()+1),
        QPoint(mapPoint.x(),mapPoint.y()-1),
        QPoint(mapPoint.x(),mapPoint.y()+1),
        QPoint(mapPoint.x()+1,mapPoint.y()-1),
        QPoint(mapPoint.x()+1,mapPoint.y()),
        QPoint(mapPoint.x()+1,mapPoint.y()+1),
        };
    for(int i = 0;i < 8;i++)
    {
        if(mapList[points[i].x()][points[i].y()]->inherits(className.toLatin1().data()))
        {
            return true;
        }
    }
    return false;
}

Player *monopolyGame::nextPlayer()
{
    int index = playerList.indexOf(runningPlayer);
    int i = 1;
    while(1)
    {
        //如果是最后一个玩家下标
        if(index+i >= PlayerNum)
        {
            //从零开始
            i = -index;
        }

        if(playerList[index+i]->stateController.isNormal())
        {
            return playerList[index+i];
            break;
        }
        i++;
    }
}


bool monopolyGame::moveCamera()
{
    QObject* sender = QObject::sender();
    QString signalName = sender->objectName();

    QPoint point(camearCenter->x(),camearCenter->y());
    switch (signalName.at(0).unicode()) {
    case 'W':
        camearCenter->setY(camearCenter->y()-1<ScreenHElems/2?ScreenHElems/2:camearCenter->y()-1);
        break;
    case 'A':
        camearCenter->setX(camearCenter->x()-1>ScreenWElems/2?camearCenter->x()-1:ScreenWElems/2);
        break;
    case 'S':
        camearCenter->setY(camearCenter->y()+1>GamepannelRow-ScreenHElems/2?GamepannelRow-ScreenHElems/2:camearCenter->y()+1);
        break;
    case 'D':
        camearCenter->setX(camearCenter->x()+1<GamepannelCol-ScreenWElems/2?camearCenter->x()+1:GamepannelCol-ScreenWElems/2);
        break;
    default:
        break;
    }

    if(point.x()!=camearCenter->x() || point.y()!=camearCenter->y())
    {
        //清空原屏幕layout
        for(int i = point.y()-ScreenHElems/2; i < point.y()+ScreenHElems/2;i++)
        {
            for(int j = point.x() - ScreenWElems/2 ; j < point.x() + ScreenWElems/2;j++)
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
    playerRun(QRandomGenerator::global()->bounded(1,6));

    return true;
}

bool monopolyGame::playerRun(int step)
{
    if(runningPlayer->stateController.isActionable())
    {
        //开始执行时让回合结束按钮关闭，防止动画出现意外
        ui->gamePannel->findChild<QPushButton*>("nextPlayer")->setEnabled(false);

        runningPlayer->steps = step;

        ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(false);

        // 定义一个定时器，用于延时显示每一步的效果
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            int x = runningPlayer->gamemapPos.x();
            int y = runningPlayer->gamemapPos.y();
            Road *road = dynamic_cast<Road*>(mapList[x][y]);
            if(runningPlayer->steps != 0 && runningPlayer->steps >= road->getStepCost())
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
                runningPlayer->steps -=road->getStepCost();

                showPlayer();

            }
            else
            {
             //玩家步数不足以走下一步或者已经走完
                runningPlayer->steps = 0;
                emit runningPlayer->playerRun();
                timer->stop();
                delete timer;
                ui->gamePannel->findChild<QPushButton*>("nextPlayer")->setEnabled(true);
            }
        });

        // 设置定时器的时间间隔，控制每一步的延时
        timer->setInterval(800); // 设置延时
        timer->start();
    }
    else
    {
        ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(false);

        QLabel *sleepyLab = new QLabel(ui->gamePannel);
        sleepyLab->setText("冬眠状态无法行动");

        //设置字体颜色
        sleepyLab->setStyleSheet("color:rgb(255,100,100);");

        sleepyLab->setGeometry(runningPlayer->x(),runningPlayer->y(),100,20);
        sleepyLab->show();

        QPropertyAnimation *animation = new QPropertyAnimation(sleepyLab,"geometry");
        animation->setDuration(1000);
        animation->setStartValue(QRect(sleepyLab->geometry().x()-15,sleepyLab->geometry().y() + runningPlayer->pixmap().height()/2,sleepyLab->geometry().width(),sleepyLab->geometry().height()));
        animation->setEndValue(QRect(sleepyLab->geometry().x()-15,sleepyLab->geometry().y() + runningPlayer->pixmap().height()/2 -10,sleepyLab->geometry().width(),sleepyLab->geometry().height()));
        animation->setEasingCurve(QEasingCurve::Linear);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        QTimer::singleShot(1000,this,[=](){
            delete sleepyLab;
            emit endOfTurn();
        });
    }

    return true;
}


bool monopolyGame::endOfTurn()
{
    checkState();
    ui->gamePannel->findChild<QPushButton*>("nextPlayer")->setEnabled(false);

    //是本回合最后一个玩家行动结束，就进入下一回合
    if(playerList.indexOf(runningPlayer) == PlayerNum-1)
    {
        roundController.nextRound();
        runningPlayer->stateController.refreshAffect(roundController.getRound());
    }

    switch (stateController.getState()) {
    case gameState::Running:
        ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(true);
        runningPlayer = nextPlayer();
        emit runningPlayer->playerChanged();
        break;
    case gameState::End:
        ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(false);
        ui->gamePannel->findChild<QPushButton*>("nextPlayer")->setEnabled(false);
        break;
    case gameState::Ready:
        break;
    }
    return true;
}
