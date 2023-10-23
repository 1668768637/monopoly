#ifndef MONOPOLYGAME_H
#define MONOPOLYGAME_H

#include <QWidget>
#include <QPixmap>
#include "QObject"
#include "AbstractMap.h"
#include "player.h"
#include "gamestatecontroller.h"
#include "gameroundcontroller.h"
#include "musicthread.h"
#include "QRandomGenerator"
#include "SocketThread.h"
#include <QTcpSocket>
#include <QMutex>
#include <QKeyEvent>

class Player;
class MapFactory;

namespace Ui {
class monopolyGame;
}

class monopolyGame : public QWidget
{
    Q_OBJECT

public:
    Ui::monopolyGame *ui;
    static monopolyGame *myWindow;

    explicit monopolyGame(QWidget *parent = 0);
    ~monopolyGame();

    QPoint *camearCenter;
    QList<QList<AbstractMap*>> mapList;
    QList<Player*> playerList;
    QList<QLabel*> playerTitleList;
    Player* runningPlayer;
    GameStateController stateController;
    GameRoundController roundController;

    //元素的像素值
    int ElemW;
    int ElemH;
    //画面大小
    int ScreenWElems;
    int ScreenHElems;
    //地图大小
    int GamepannelRow;
    int GamepannelCol;
    //默认相机中心
    int CameracenterX;
    int CameracenterY;
    //游戏人数
    int PlayerNum;
    //玩家初始金币数量
    float initPlayerMoneyNum;
    //玩家名字列表
    QVariantList playerNameList;
    //游戏背景音乐线程
    MusicThread* musicThread;
    //链接服务器线程
    SocketThread* socketThread;

    bool initGame();

    bool printMap();
    bool refreshState();
    //判断游戏地图中mapPoint位置的方块周围是否有className类
    bool isAroundClass(QPoint mapPoint,QString className);

    /**
     * @brief 找到下一个玩家
     *
     * 函数详细描述
     *
     *
     * @return 返回一个玩家的指针
     */
    Player* nextPlayer();

private:
    //Rendering Players
    bool showPlayer();
    bool openKnapsack();
    bool clickRunButton();

    //读取游戏各项设置
    bool initSettings();
    bool initSettings(QString settingsPath);
    //在内存中生成地图
    bool initGameMap();
    bool initGameMap(QString mapPath);
    //生成游戏所需的数据
    bool initGameData();
    bool initGameData(QString mapPath);//需要地图文件是为了找到开始时的坐标，等待优化
    //生成地图之后检查各种方块位置限制和数据调整，保证符合游戏逻辑
    bool checkMapLogic();
    bool initGameUi();
    bool showMainUI();
    bool initMainUI();
    bool initPerpareUI();

protected:
    void keyPressEvent(QKeyEvent *event)override;

signals:
    void beginInit_local();
    void beginInit_CS();
    void initStepAdd();

public slots:
    bool moveCamera();
    bool playerRun();
    bool playerRun(int step);
    bool endOfTurn();
};

#endif // MONOPOLYGAME_H
