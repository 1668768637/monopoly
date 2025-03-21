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
#include "Money.h"
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
#include "sleepcard.h"
#include "userinfogroup.h"
#include <QScrollArea>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QProgressBar>
#include <QThread>
#include <QOverload>
#include <shop.h>
#include <GamePushButton.h>
#include <QUiLoader>
#include <log.h>
#include <JsonTool.h>

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

    //默认本地游戏
    this->isCSGame = false;

    showMainUI();


    connect(this,&monopolyGame::initStepAdd,this,[=](){
        QProgressBar *progressBar = ui->prepareUI->findChild<QProgressBar*>("initBar");
        int value = progressBar->value();
        if(value+1 <= progressBar->maximum())
            progressBar->setValue(value+1);
    });

    connect(this,&monopolyGame::beginGame,this,[=](bool isCSGame){
        this->startGame(isCSGame);
        },Qt::QueuedConnection);

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
    //本地游戏按钮
    QPushButton *startGameBtn_local = new GamePushButton(ui->mainUI);
    startGameBtn_local->setGeometry(0,0,150,50);
    startGameBtn_local->move(ui->mainUI->width()/2-startGameBtn_local->width()/2-50-startGameBtn_local->width(),ui->mainUI->height()*0.75);
    startGameBtn_local->setText("本地游戏");
    QObject::connect(startGameBtn_local,&QPushButton::clicked,[=](){
        startGame(false);
    });

    //C/S模式按钮游戏按钮
    QPushButton *startGameBtn_CS = new GamePushButton(ui->mainUI);
    startGameBtn_CS->setObjectName("startGameBtn_CS");
    startGameBtn_CS->setGeometry(0,0,150,50);
    startGameBtn_CS->move(ui->mainUI->width()/2-startGameBtn_CS->width()/2+50+startGameBtn_CS->width(),ui->mainUI->height()*0.75);
    startGameBtn_CS->setText("联机游戏");
    startGameBtn_CS->setEnabled(false);
    QObject::connect(startGameBtn_CS,&QPushButton::clicked,[=](){
        startGameBtn_CS->setText("正在匹配");
        startGameBtn_CS->setEnabled(false);
        QJsonObject *request = new QJsonObject();
        request->insert("type","beginWaiting");
        emit this->socketThread->sendMessageSignal(request);
    });

    //初始化tcp线程
    this->socketThread = new SocketThread(this,serverIP,1000);
    //暂时关闭socket功能，暂停网络开发
    //socketThread->start();
    connect(socketThread,&SocketThread::RunThread,this,[=](QThread* runThread){
        socketThread->moveToThread(runThread);
    });
    connect(socketThread,&SocketThread::connectSuccessful,this,[=](){
//        startGameBtn_CS->setEnabled(true);socket连接成功并不能立即开始匹配，必须要登陆成功后才可以
        ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->setEnabled(true);
    });
    //断开链接响应
    connect(socketThread,&SocketThread::connectBroken,this,[=](){
        //关闭联机模式按钮
        startGameBtn_CS->setEnabled(false);
        startGameBtn_CS->setText("开始匹配");
        this->findChild<QLabel*>("delayLab")->setText("Null");
        //显示登录按钮
        ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->setEnabled(false);
        ui->mainUI->findChild<QPushButton*>("logLaunchBtn")->show();
        //隐藏玩家信息标签
        ui->mainUI->findChild<QGroupBox*>("userinfoGroup")->hide();
        ui->mainUI->findChild<QWidget*>("logWidget")->hide();
    });
    //用户走一步响应
    connect(socketThread,&SocketThread::playerRun,this,[=](int steps){
        playerRun(steps);
    });

    //标题图标
    QLabel *title = new QLabel(ui->mainUI);
    QPixmap titleImg(":/res/img/title.png");
    titleImg = titleImg.scaledToWidth(800);
    title->setPixmap(titleImg);
    title->move(ui->mainUI->width()/2-titleImg.width()/2,20);

    //游戏设置按钮

    //延迟图标
    QLabel *delayLab = new QLabel(this);
    delayLab->setGeometry(0,0,50,25);
    delayLab->setObjectName("delayLab");
    delayLab->setText("Null");
    connect(socketThread,&SocketThread::setCurrentDelay,this,[=](int ms){
        delayLab->setText(QString::number(ms)+"ms");
    });


    //登录界面弹出按钮
    //生成登录界面并隐藏
    class log *logWidget = new class log(ui->mainUI);
    logWidget->hide();
    int height = logWidget->height();
    int width = logWidget->width();
    int UIheight = ui->mainUI->height();
    int UIwidth = ui->mainUI->width();
    logWidget->setGeometry(UIwidth/2-width/2,UIheight/2-height/2,width,height);
    connect(logWidget,&log::login,this,[=](QJsonObject *request){
        emit socketThread->sendMessageSignal(request);
    });

    QPushButton *logLaunchBtn = new QPushButton(ui->mainUI);
    logLaunchBtn->setObjectName("logLaunchBtn");
    logLaunchBtn->setText("登录");
    logLaunchBtn->setGeometry(ui->mainUI->width()-70,0,50,25);
    logLaunchBtn->setEnabled(false);
    connect(logLaunchBtn,&QPushButton::clicked,this,[=](){
        logWidget->show();
        ui->mainUI->findChild<QWidget*>("logWidget")->findChild<QLabel*>("logErrorMsgLabel")->setText("");
    });

    //玩家信息group
    userinfoGroup *userinfo = new userinfoGroup(ui->mainUI);
    userinfo->setGeometry(ui->mainUI->width() - userinfo->width(),0,userinfo->width(),userinfo->height());
    userinfo->hide();
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

bool monopolyGame::initGame(bool isCSGame)
{
    QString mapFilePath,roominfoFilePath;
    if(isCSGame)
    {
        mapFilePath = "./csFile/map.txt";
        roominfoFilePath = "./csFile/roominfo.json";
    }
    else
    {
        mapFilePath = "./localFile/map.txt";
        roominfoFilePath = "./localFile/roominfo.json";
    }
    QLabel *lab = ui->prepareUI->findChild<QLabel*>("initLab");

    lab->setText("正在初始化游戏数据");
    if(!initGameData(mapFilePath,roominfoFilePath))return false;
    emit initStepAdd();
    lab->setText("正在初始化游戏地图");
    if(!initGameMap(mapFilePath))return false;
    emit initStepAdd();
    lab->setText("正在初始化游戏UI");
    if(!initGameUi())return false;
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
    serverIP = settingsObj["serverIP"].toString();

    settingsFile->close();

    stateController.setState(gameState::Ready);

    //默认开启游戏声音
    musicThread = new MusicThread();
    musicThread->start();

    return true;
}

bool monopolyGame::initGameMap(QString mapPath)
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
    QFile map(mapPath);
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

bool monopolyGame::initGameData(QString mapPath,QString roominfoPath)
{
    //读取游戏必要参数
    QFile roomSetting(roominfoPath);
    if(!roomSetting.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::information(this,"游戏文件错误","房间配置文件不存在！");
        return false;
    }
    QJsonObject roomSettingJson = JsonTool::stringToJson(roomSetting.readAll());
    ElemW = roomSettingJson["ElemW"].toInt();
    ElemH = roomSettingJson["ElemH"].toInt();
    ScreenWElems = roomSettingJson["ScreenWElems"].toInt();
    ScreenHElems = roomSettingJson["ScreenHElems"].toInt();
    GamepannelRow = roomSettingJson["GamepannelRow"].toInt();
    GamepannelCol = roomSettingJson["GamepannelCol"].toInt();
    CameracenterX = roomSettingJson["CameracenterX"].toInt();
    CameracenterY = roomSettingJson["CameracenterY"].toInt();
    PlayerNum = roomSettingJson["PlayerNum"].toInt();
    initPlayerMoneyNum = roomSettingJson["initPlayerMoneyNum"].toDouble();
    playerNameList = roomSettingJson["playerNameList"].toArray().toVariantList();


    if(playerNameList.count() != PlayerNum)
    {
        QMessageBox::information(this,"游戏文件错误","游戏玩家信息不匹配：玩家数量和玩家名字数量不一致\n！");
        return false;
    }

    //设置相机中心
    camearCenter = new QPoint(CameracenterX,CameracenterY);

    //绑定按键调整镜头
    QShortcut *shortcut_W = new QShortcut(QKeySequence(Qt::Key_W), this);
    shortcut_W->setObjectName("W");
    connect(shortcut_W, &QShortcut::activated, this, &monopolyGame::moveCamera);
    QShortcut *shortcut_S = new QShortcut(QKeySequence(Qt::Key_S), this);
    shortcut_S->setObjectName("S");
    connect(shortcut_S, &QShortcut::activated, this, &monopolyGame::moveCamera);
    QShortcut *shortcut_A = new QShortcut(QKeySequence(Qt::Key_A), this);
    shortcut_A->setObjectName("A");
    connect(shortcut_A, &QShortcut::activated, this, &monopolyGame::moveCamera);
    QShortcut *shortcut_D = new QShortcut(QKeySequence(Qt::Key_D), this);
    shortcut_D->setObjectName("D");
    connect(shortcut_D, &QShortcut::activated, this, &monopolyGame::moveCamera);

    QPoint begin;

    QFile map(mapPath);
    if (!map.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false; // 如果无法打开文件，则退出
    }
    QTextStream ins(&map);
    if(ins.atEnd())
    {
        QMessageBox::information(this,"游戏文件无效","初始化游戏地图时遇到了无效数据");
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

    map.close();

    //生成玩家
    for(int num = 0; num < PlayerNum;num++)
    {
        playerList.append(new Player(begin,initPlayerMoneyNum));
        playerList.at(num)->setName(playerNameList.at(num).toString());
        playerList.at(num)->setParent(ui->gamePannel);
        if(roomSettingJson.contains("IDList"))
        {
            playerList.at(num)->setId(roomSettingJson.value("IDList").toArray().at(num).toString());
        }
        //IDList的第一个就是首先行动的玩家
        if(playerList.at(num)->getId() == myID)
        {
            myself = playerList.at(num);
        }

        //生成头衔
        QLabel *lab = new QLabel(ui->gamePannel);
        lab->setText(playerList.at(num)->getName());
        lab->setGeometry(0,0,100,30);
        lab->setStyleSheet("font-size:20px;");
        playerTitleList.append(lab);

        Money *userMoney = dynamic_cast<Money*>(playerList[num]->getKnapsack()->getProp("Money"));

        //绑定金币改变事件
        connect(userMoney,&Money::moneyChanged,[=](){
            Money *runningUserMoney = dynamic_cast<Money*>(runningPlayer->getKnapsack()->getProp("Money"));
            ui->gamePannel->findChild<QLabel*>("runningPlayer_moneyLab")->setText("余额：" + QString::number(runningUserMoney->getNum()));

            //更新玩家控制器
            if(runningUserMoney->getNum() < 0)runningPlayer->getStateController()->setState(PlayerState::Bankruptcy);
        });


        //绑定行走完毕事件
        connect(playerList[num],&Player::playerRun,[=](){

            //扣除租金
            Road *road = dynamic_cast<Road*>(mapList[runningPlayer->getGamemapPos().x()][runningPlayer->getGamemapPos().y()]);
            //找到这个道路对应的两个可能的房子
            QList<House*> houseList;
            House* house;
            switch (road->getDirection()) {
            case Road::Direct::LEFT:
            case Road::Direct::RIGHT:
                if(mapList[runningPlayer->getGamemapPos().x()-1][runningPlayer->getGamemapPos().y()]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->getGamemapPos().x()-1][runningPlayer->getGamemapPos().y()]);
                    if(house->getOwner() != runningPlayer)houseList.append(house);
                }
                if(mapList[runningPlayer->getGamemapPos().x()+1][runningPlayer->getGamemapPos().y()]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->getGamemapPos().x()+1][runningPlayer->getGamemapPos().y()]);
                    if(house->getOwner() != runningPlayer)houseList.append(house);
                }
                break;
            case Road::Direct::UP:
            case Road::Direct::DOWN:
                if(mapList[runningPlayer->getGamemapPos().x()][runningPlayer->getGamemapPos().y()-1]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->getGamemapPos().x()][runningPlayer->getGamemapPos().y()-1]);
                    if(house->getOwner() != runningPlayer)houseList.append(house);
                }
                if(mapList[runningPlayer->getGamemapPos().x()][runningPlayer->getGamemapPos().y()+1]->inherits("House"))
                {
                    house = dynamic_cast<House*>(mapList[runningPlayer->getGamemapPos().x()][runningPlayer->getGamemapPos().y()+1]);
                    if(house->getOwner() != runningPlayer)houseList.append(house);
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
                QPushButton *confirmBtn = new GamePushButton(msg);

                lab->setStyleSheet("font-size:20px;");

                lab->move(50,0);
                confirmBtn->move(75,75);

                lab->setText("扣除租金："+QString::number(houseList[i]->getRent()));
                confirmBtn->setText("确认");

                connect(confirmBtn,&QPushButton::clicked,[=](){
                    msg->close();
                    delete lab;
                    delete confirmBtn;
                    delete msg;
                });

                msg->show();

                //获取金币指针
                Money *runningPlayerMoney = dynamic_cast<Money*>(runningPlayer->getKnapsack()->getProp("Money"));
                runningPlayerMoney->reduce(houseList[i]->getRent());
                dynamic_cast<Money*>(houseList[i]->getOwner()->getKnapsack()->getProp("Money"))->add(houseList[i]->getRent());
                emit runningPlayerMoney->moneyChanged();
            }
        });


        //绑定行走玩家发生变化事件
        connect(playerList[num],&Player::playerChanged,[=](){
            QLabel* lab = ui->gamePannel->findChild<QLabel*>("runningPlayerLab");
            lab->setText(runningPlayer->getName());

            Money *runningUserMoney = dynamic_cast<Money*>(runningPlayer->getKnapsack()->getProp("Money"));
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
            //黑洞附近消耗步数为2
            if(dynamic_cast<Road*>(list.at(j))!=nullptr && isAroundClass(list.at(j)->gamemapPos,"BlackHole"))
            {
               Road* p = dynamic_cast<Road*>(list.at(j));
               p->setStepCost(2);
            }
        }
    }
    return true;
}


bool monopolyGame::initGameUi()
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
    QPushButton *run = new GamePushButton(ui->gamePannel);
    run->setObjectName("run");
    run->setText("走一次");
    run->move(ui->gamePannel->width()-250,ui->gamePannel->height()-100);
    run->show();
    connect(run,&QPushButton::clicked,this,QOverload<>::of(&playerRun));

    //回合结束按钮
    QPushButton *endOfTurn = new GamePushButton(ui->gamePannel);
    endOfTurn->setObjectName("nextPlayer");
    endOfTurn->setText("回合结束");
    endOfTurn->move(ui->gamePannel->width()-150,ui->gamePannel->height()-100);
    endOfTurn->setEnabled(false);
    endOfTurn->show();
    connect(endOfTurn,&QPushButton::clicked,this,&monopolyGame::endOfTurn);

    //背包按钮
    QPushButton *knapsackBtn = new GamePushButton(ui->gamePannel);
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
            QPushButton *knapsackUI_closeBtn = new GamePushButton(headBox);
            knapsackUI_closeBtn->setObjectName("knapsackUI_closeBtn");
            knapsackUI_closeBtn->setCursor(Qt::PointingHandCursor);
            knapsackUI_closeBtn->setText("X");
            connect(knapsackUI_closeBtn,&QPushButton::clicked,[=](){
                knapsackUI->hide();
            });
            //背包信息
            QLabel *knapsackUI_knapsackInfo = new QLabel(headBox);
            knapsackUI_knapsackInfo->setText(runningPlayer->getName()+"的背包");
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
            propList->setWidget(runningPlayer->getKnapsack()->getTab());

            knapsackUI->show();
        }
        else
        {
            //更新背包信息为当前玩家的背包
            QGroupBox* headUI = ui->gamePannel->findChild<QGroupBox*>("knapsackUI")->findChild<QGroupBox*>("headBox");
            QGroupBox* bodyUI = ui->gamePannel->findChild<QGroupBox*>("knapsackUI")->findChild<QGroupBox*>("bodyBox");

            headUI->findChild<QLabel*>("knapsackUI_knapsackInfo")->setText(runningPlayer->getName()+"的背包");
            bodyUI->findChild<QScrollArea*>("propList")->setWidget(runningPlayer->getKnapsack()->getTab());
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
    runningPlayerLab->setText(runningPlayer->getName());
    runningPlayerLab->setGeometry(0,0,200,50);
    runningPlayerLab->show();
    //余额label
    QLabel *runningPlayer_moneyLab = new QLabel(runningPlayerInfo);
    runningPlayer_moneyLab->setObjectName("runningPlayer_moneyLab");
    runningPlayer_moneyLab->setText("余额：" +QString::number(dynamic_cast<Money*>(runningPlayer->getKnapsack()->getProp("Money"))->getNum()));
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
            playerList.at(i)->getStateController()->refreshAffect(roundController.getRound());
        }
    });
    runningPlayerInfo->show();


    //游戏信息按钮
    QPushButton *info = new GamePushButton(ui->gamePannel);
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

        QPushButton *confirm = new GamePushButton(infoBox);
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

bool monopolyGame::refreshState()
{
    int bankruptcyNum = 0;
    for(int num = 0; num < playerList.length();num++)
    {
        if(playerList[num]->getStateController()->getState() == PlayerState::Bankruptcy)
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
               if(this->playerList[num]->getGamemapPos().x() == i && this->playerList[num]->getGamemapPos().y() == j)
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

bool monopolyGame::openKnapsack()
{
    QGroupBox* knapsackUI = ui->gamePannel->findChild<QGroupBox*>("knapsackUI");
    //指针为空，说明内容还没有创建，必须要通过点击背包按钮的方式进行创建内容;或是游戏未开始，需要分别判断
    if(knapsackUI == nullptr)
    {
        if(this->stateController.isRunning())
        {
            QPushButton* knapsackBtn =  ui->gamePannel->findChild<QPushButton*>("knapsackBtn");
            knapsackBtn->click();
        }
    }
    else
    {
        if(knapsackUI->isHidden())
        {
            QPushButton* knapsackBen =  ui->gamePannel->findChild<QPushButton*>("knapsackBtn");
            knapsackBen->click();
        }
        else
        {
            knapsackUI->hide();
        }
    }
    return true;
}

bool monopolyGame::clickRunButton()
{
    GamePushButton* runBtn = ui->gamePannel->findChild<GamePushButton*>("run");
    //指针为空，说明游戏还没开始
    if(runBtn == nullptr)
    {
        qDebug()<<"game isn't running!!!";
    }
    else
    {
        if(runBtn->isEnabled())runBtn->click();
    }
    return true;
}

void monopolyGame::keyPressEvent(QKeyEvent *event)
{
    //control组合键模式
    if(event->modifiers()==Qt::ControlModifier)
    {
        event->ignore();
    }
    else
    {
        //单个按键模式
        switch(event->key())
        {
            //屏蔽tab键、空格键
        case Qt::Key_Space:
        case Qt::Key_Tab:
            break;

            //B键打开背包
        case Qt::Key_B:
            openKnapsack();
            break;
        case Qt::Key_Enter:
            clickRunButton();
            break;
        default:
            QWidget::keyPressEvent(event);
        }
    }
}

bool monopolyGame::isAroundClass(QPoint mapPoint,QString className)
{
    //附近八个方块的点阵
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
        if(points[i].x()<0 && points[i].y()<0) return false;
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

        if(playerList[index+i]->getStateController()->isNormal())
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
    if(isCSGame)
    {
        QJsonObject *request = new QJsonObject();
        request->insert("type","run");
        request->insert("runPlayerID",runningPlayer->getId());
        this->socketThread->sendMessage(request);
    }
    else
    {
        playerRun(QRandomGenerator::global()->bounded(1,6));
    }

    return true;
}

bool monopolyGame::playerRun(int step)
{
    if(runningPlayer->getStateController()->isActionable())
    {
        //开始执行时让回合结束按钮关闭，防止动画出现意外
        ui->gamePannel->findChild<QPushButton*>("nextPlayer")->setEnabled(false);

        runningPlayer->setSteps(step);

        ui->gamePannel->findChild<QPushButton*>("run")->setEnabled(false);

        // 定义一个定时器，用于延时显示每一步的效果
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, [=]() {
            int x = runningPlayer->getGamemapPos().x();
            int y = runningPlayer->getGamemapPos().y();
            Road *road = dynamic_cast<Road*>(mapList[x][y]);
            if(runningPlayer->getSteps() != 0 && runningPlayer->getSteps() >= road->getStepCost())
            {
                switch (road->getDirection()) {
                    case Road::Direct::RIGHT:
                        runningPlayer->getGamemapPos().setY(y+1);
                        break;
                    case Road::Direct::LEFT:
                        runningPlayer->getGamemapPos().setY(y-1);
                        break;
                    case Road::Direct::UP:
                        runningPlayer->getGamemapPos().setX(x-1);
                        break;
                    case Road::Direct::DOWN:
                        runningPlayer->getGamemapPos().setX(x+1);
                        break;
                    default:
                        break;
                }
                runningPlayer->setSteps(runningPlayer->getSteps() - road->getStepCost());

                showPlayer();

            }
            else
            {
             //玩家步数不足以走下一步或者已经走完
                runningPlayer->setSteps(0);
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

    //无论如何，玩家都已经执行完行走逻辑
    emit runningPlayer->playerRun();

    return true;
}


bool monopolyGame::endOfTurn()
{
    refreshState();
    ui->gamePannel->findChild<QPushButton*>("nextPlayer")->setEnabled(false);

    //是本回合最后一个玩家行动结束，就进入下一回合
    if(playerList.indexOf(runningPlayer) == PlayerNum-1)
    {
        roundController.nextRound();
        runningPlayer->getStateController()->refreshAffect(roundController.getRound());
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

bool monopolyGame::startGame(bool isCSGame)
{
    ui->mainUI->hide();
    ui->prepareUI->show();
    initGame(isCSGame);
    stateController.setState(gameState::Running);

    //test area
    runningPlayer->getKnapsack()->addProp(new SleepCard());

    ui->prepareUI->hide();
    ui->gamePannel->show();

    return true;
}
