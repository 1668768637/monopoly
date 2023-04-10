#include "house.h"
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>

House::House()
{
    setObjectName("House");
    this->setPixmap(QPixmap(":/res/img/house.png"));
    owner = nullptr;
    rent = 0.0;

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
