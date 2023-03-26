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

monopolyGame::monopolyGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monopolyGame)
{
    ui->setupUi(this);

    this->setWindowTitle(QString("大富翁"));
    ui->gamePannel->setGeometry(0,0,1920,1020);
    ui->map->setGeometry(0,0,1920,1020);
    initGameMap();


    //测试代码，完成后删除

}

monopolyGame::~monopolyGame()
{
    delete ui;
}

bool monopolyGame::initGameMap()
{
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setSpacing(0);
    for(int i = 0;i < GAMEPANNEL_COL;i++)
    {
        QVBoxLayout *innerLay = new QVBoxLayout();
        innerLay->setSpacing(0);
        for(int j = 0;j < GAMEPANNEL_ROW;j++)
        {

            QLabel *lab = new QLabel(ui->map);
            QPixmap *pixmap = new QPixmap(":/res/img/road.png");
            lab->setPixmap(*pixmap);
            innerLay->addWidget(lab);
        }
        hl->addLayout(innerLay);
    }
    ui->map->setLayout(hl);
    return true;
}
