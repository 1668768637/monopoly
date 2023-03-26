#ifndef MONOPOLYGAME_H
#define MONOPOLYGAME_H

#include <QWidget>
#include "QObject"

#define GAMEPANNEL_ROW 20
#define GAMEPANNEL_COL 50
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
    bool initGameMap();
};

#endif // MONOPOLYGAME_H
