#ifndef MONOPOLYGAME_H
#define MONOPOLYGAME_H

#include <QWidget>
#include <QPixmap>
#include "QObject"
#include "AbstractMap.h"
#include "player.h"
#include "gamestatecontroller.h"
#include "gameroundcontroller.h"

/*
//元素的像素值
#define ELEM_W 36
#define ELEM_H 51

//画面大小
#define SCREEN_W_ELEMS 50
#define SCREEN_H_ELEMS 20

//地图大小
#define GAMEPANNEL_ROW 20
#define GAMEPANNEL_COL 50

//默认相机中心
#define CAMERACENTER_X GAMEPANNEL_COL/2
#define CAMERACENTER_Y GAMEPANNEL_ROW/2

//游戏人数
#define PLAYER_NUM 2
*/
class Player;

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
    int ELEM_W;
    int ELEM_H;

    //画面大小
    int SCREEN_W_ELEMS;
    int SCREEN_H_ELEMS;

    //地图大小
    int GAMEPANNEL_ROW;
    int GAMEPANNEL_COL;

    //默认相机中心
    int CAMERACENTER_X;
    int CAMERACENTER_Y;

    //游戏人数
    int PLAYER_NUM;

    bool initSettings();
    bool initGameMap();
    bool initGameData();
    bool initUI();
    bool printMap();
    bool checkState();

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
    bool showPlayer();

public slots:
    bool moveCamera();
    bool playerRun();
    bool endOfTurn();
};

#endif // MONOPOLYGAME_H
