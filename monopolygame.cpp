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

monopolyGame::monopolyGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monopolyGame)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("大富翁"));
    ui->gamePannel->setGeometry(0,0,1800,969);
    ui->map->setGeometry(0,0,1800,969);
    camearCenter = new QPoint(CAMERACENTER_X,CAMERACENTER_Y);

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


    //测试代码，完成后删除

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
    QFile map("D:\\Desktop\\QT\\Monopoly\\Monopoly\\map\\map.txt");
    //QFile map(":/res/map/map.txt");
    if (!map.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"打开失败";
        return false; // 如果无法打开文件，则退出
    }


    QTextStream ins(&map);
    while(!ins.atEnd())
    {
        QStringList strList =  ins.readLine().split(" ");
        qDebug()<<strList;
        gameMap[strList[0].toInt()][strList[1].toInt()] = strList[2].toInt();
        qDebug()<<strList[0].toInt()<<" "<<strList[1].toInt()<<" "<<strList[2].toInt();
    }
    qDebug()<<"读取完成";


    //生成地图
    mapList.clear();
    for(int i = 0;i < GAMEPANNEL_ROW;i++)
    {
        mapList.append(QList<AbstractMap*>());
        for(int j = 0;j < GAMEPANNEL_COL;j++)
        {
            AbstractMap* p = MapFactory::createMap(gameMap[i][j]);
            mapList[i].append(p);
        }
    }

    printMap();
    return true;
}


bool monopolyGame::initUI()
{
    return true;
}

bool monopolyGame::printMap()
{
    //清空layout
    delete ui->map->layout();

    QGridLayout *girdLayout = new QGridLayout(ui->map);
    girdLayout->setSpacing(0);
    for(int i = camearCenter->y()-SCREEN_H_ELEMS/2,girdI = 0 ; i < camearCenter->y()+SCREEN_H_ELEMS/2;i++,girdI++)
    {
        for(int j = camearCenter->x() - SCREEN_W_ELEMS/2 ,girdJ = 0; j < camearCenter->x() + SCREEN_W_ELEMS/2;j++,girdJ++)
        {
            AbstractMap* p = mapList[i][j];
            girdLayout->addWidget(p,girdI,girdJ);
            p->update();
        }
    }
    ui->map->setLayout(girdLayout);
    return true;
}

bool monopolyGame::moveCamera()
{
    QObject* sender = QObject::sender();
    QString signalName = sender->objectName();

    switch (signalName.at(0).unicode()) {
    case 'W':
        camearCenter->setY(camearCenter->y()-1<10?10:camearCenter->y()-1);
        break;
    case 'A':
        camearCenter->setX(camearCenter->x()-1>25?camearCenter->x()-1:25);
        break;
    case 'S':
        camearCenter->setY(camearCenter->y()+1>50?50:camearCenter->y()+1);
        break;
    case 'D':
        camearCenter->setX(camearCenter->x()+1<34?camearCenter->x()+1:34);
        break;
    default:
        break;
    }
    printMap();
    return true;
}
