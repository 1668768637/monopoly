#ifndef MONOPOLYGAME_H
#define MONOPOLYGAME_H

#include <QWidget>
#include "QObject"
#include "AbstractMap.h"
#include "player.h"


//默认相机中心
#define CAMERACENTER_X 28
#define CAMERACENTER_Y 10

//元素的像素值
#define ELEM_W 36
#define ELEM_H 51

//画面大小
#define SCREEN_W_ELEMS 50
#define SCREEN_H_ELEMS 20

//地图大小
#define GAMEPANNEL_ROW 60
#define GAMEPANNEL_COL 60

//游戏人数
#define PLAYER_NUM 2
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

    bool initGameMap();
    bool initGameData();
    bool initUI();
    bool printMap();
    Player* nextPlayer();

public slots:
    bool moveCamera();
    bool playerRun();
    bool endOfTurn();
};

#endif // MONOPOLYGAME_H
