#ifndef MONOPOLYGAME_H
#define MONOPOLYGAME_H

#include <QWidget>
#include "QObject"
#include "AbstractMap.h"

#define CAMERACENTER_X 30
#define CAMERACENTER_Y 30

#define SCREEN_W_ELEMS 50
#define SCREEN_H_ELEMS 20

#define GAMEPANNEL_ROW 60
#define GAMEPANNEL_COL 60
namespace Ui {
class monopolyGame;
}

class monopolyGame : public QWidget
{
    Q_OBJECT

public:
    explicit monopolyGame(QWidget *parent = 0);
    ~monopolyGame();

private:
    Ui::monopolyGame *ui;
    QPoint *camearCenter;
    QList<QList<AbstractMap*>> mapList;

    bool initGameMap();
    bool initUI();
    bool printMap();

public slots:
    bool moveCamera();
};

#endif // MONOPOLYGAME_H
