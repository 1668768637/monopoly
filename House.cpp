#include "house.h"
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include "land.h"
#include "blackhole.h"

bool House::isAroundBlackhole()
{
    QPoint points[8] = {
        QPoint(this->gamemapPos.x()-1,this->gamemapPos.y()-1),
        QPoint(this->gamemapPos.x()-1,this->gamemapPos.y()),
        QPoint(this->gamemapPos.x()-1,this->gamemapPos.y()+1),
        QPoint(this->gamemapPos.x(),this->gamemapPos.y()-1),
        QPoint(this->gamemapPos.x(),this->gamemapPos.y()+1),
        QPoint(this->gamemapPos.x()+1,this->gamemapPos.y()-1),
        QPoint(this->gamemapPos.x()+1,this->gamemapPos.y()),
        QPoint(this->gamemapPos.x()+1,this->gamemapPos.y()+1),
        };
    for(int i = 0;i < 8;i++)
    {
        if(dynamic_cast<BlackHole*>(gameWindow->mapList[points[i].x()][points[i].y()]) != nullptr)
        {
            return true;
        }
    }
    return false;
}

House::House(Player *owner,int x,int y):OperablePlace(x,y)
{
    setObjectName("House");
    this->setPixmap(QPixmap(":/res/img/house.png"));
    this->owner = owner;

    float price = dynamic_cast<Land*>(gameWindow->mapList[x][y])->price;
    if(this->isAroundBlackhole())
    {
        this->rent = price/2;
    }
    else
    {
        this->rent = price;
    }

    connect(this,&House::clicked,this,&showHouseInfo);
}

bool House::showHouseInfo()
{
    QGroupBox *houseInfoPannel = new QGroupBox(gameWindow->ui->gamePannel);
    int houseInfoPannel_width = 300,houseInfoPannel_height = 200;
    houseInfoPannel->setGeometry(gameWindow->ui->gamePannel->width()/2-houseInfoPannel_width/2,gameWindow->ui->gamePannel->height()/2-houseInfoPannel_height/2,houseInfoPannel_width,houseInfoPannel_width);
    QLabel *houseInfo = new QLabel(houseInfoPannel);
    houseInfo->setText(owner->name + "租金：" + QString::number(rent));

    QPushButton *confirm = new QPushButton(houseInfoPannel);
    confirm->setText("确认");
    confirm->move(50,houseInfoPannel->height()-100);


    //确认信息
    connect(confirm,&QPushButton::clicked,[=](){
        houseInfoPannel->close();
        delete confirm;
        delete houseInfoPannel;
    });

    houseInfoPannel->show();
    return true;
}
