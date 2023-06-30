#include "shop.h"
#include <QPushButton>
#include <QTime>
#include <QRandomGenerator>
#include <QScrollArea>
#include "propFactory.h"

int Shop::defaultListLength = 10;

QList<int> Shop::propCodeList = []{
    QList<int> list;

    //在这里注册各种会出现在商店中的各种道具
    list.append(100);//加入冬眠卡

    return list;
}();


QList<Goods*> Shop::goodsList = []{
    QList<Goods*> list;
    return list;
}();

void Shop::refreshShop()
{
    //利用已有的面板，将其信息更新为本商店的内容
    QGroupBox* buyPannel = gameWindow->ui->gamePannel->findChild<QGroupBox*>("shopBuyPannel");
    QGroupBox* bodyUI = buyPannel->findChild<QGroupBox*>("bodyBox");
    bodyUI->findChild<QScrollArea*>("propList")->setWidget(getPropUI());
}

Shop::Shop(int x,int y):OperablePlace(x,y)
{
    setObjectName("Shop");
    //将自身大小设置为 36x51，与实际图片大小一致
    setFixedSize(36, 51);
    this->setPixmap(QPixmap(":/res/img/shop.png"));

    connect(this,SIGNAL(clicked()),this,SLOT(showShopUI()));

    if(goodsList.length() == 0)
    {
        //随机生成各类道具
        for(int i = 0;i < defaultListLength;i++)
        {
            int code = QRandomGenerator::global()->bounded(0,Shop::propCodeList.length());
            Goods* g = new Goods(PropFactory::createProp(Shop::propCodeList[code]));
            connect(g,&Goods::boughtGoods,this,[=](){
                Goods* goods = dynamic_cast<Goods*>(sender());
                goodsList.removeAt(goodsList.indexOf(goods));
                refreshShop();
            });
            goodsList.append(g);
        }
    }
}

QWidget *Shop::getPropUI()
{
    QWidget* returnWidget = new QWidget();
    int index = 0,propNum = 0;

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();
    hl->setSpacing(45);
    hl->setContentsMargins(50,0,0,0);
    while(index < goodsList.length())
    {
        if(propNum%8==0 && propNum!=0)//一行满了
        {
            vl->addLayout(hl);
            hl = new QHBoxLayout();
            hl->setSpacing(45);
            hl->setContentsMargins(50,20,0,0);
        }


        Goods* goods = goodsList.at(index);
        hl->addWidget(goods);
        propNum++;
        index++;
    }

    hl->addStretch();
    vl->addLayout(hl);//加上最后一个没有满的
    returnWidget->setLayout(vl);
    return returnWidget;
}

bool Shop::showShopUI()
{
    //防止重复出现，导致游戏逻辑出现问题
    QGroupBox* buyPannel = gameWindow->ui->gamePannel->findChild<QGroupBox*>("shopBuyPannel");

    QPoint shopPos = dynamic_cast<Shop*>(sender())->gamemapPos;
    QPoint playerPos = gameWindow->runningPlayer->gamemapPos;
    double distance = sqrt(pow(shopPos.x()-playerPos.x(),2) + pow(shopPos.y()-playerPos.y(),2));

    if(distance <= 1.0)
    {
        if(buyPannel == nullptr)
        {
            int buyPannel_width = 1400,buyPannel_height = 800;

            //总UI
            buyPannel = new QGroupBox(gameWindow->ui->gamePannel);
            buyPannel->setGeometry(gameWindow->ui->gamePannel->width()/2-buyPannel_width/2,gameWindow->ui->gamePannel->height()/2-buyPannel_height/2,buyPannel_width,buyPannel_height);
            buyPannel->setObjectName("shopBuyPannel");

            //商店界面头部
            QGroupBox *shopBuyPannel_head = new QGroupBox(buyPannel);
            shopBuyPannel_head->setGeometry(0,0,buyPannel_width,buyPannel->height()*0.2);
            //关闭按钮
            QPushButton *closeBtn = new QPushButton(shopBuyPannel_head);
            closeBtn->setObjectName("shopCloseBtn");
            closeBtn->setText("X");
            int closeBtn_width=30,closeBtn_height=30;
            closeBtn->setGeometry(buyPannel->width()-closeBtn_width-20,25,closeBtn_width,closeBtn_height);
            connect(closeBtn,&QPushButton::clicked,[=](){
                buyPannel->hide();
            });

            //UIbody元素
            QGroupBox *bodyBox = new QGroupBox(buyPannel);
            bodyBox->setObjectName("bodyBox");
            bodyBox->setGeometry(0,shopBuyPannel_head->height(),buyPannel->width(),buyPannel->height()*0.8);
            //物品列表
            QScrollArea *propList = new QScrollArea(bodyBox);
            propList->setObjectName("propList");
            propList->setGeometry(0,0,bodyBox->width(),bodyBox->height());
            propList->setWidget(getPropUI());
        }
        else
        {
            //利用已有的面板，将其信息更新为本商店的内容
            QGroupBox* bodyUI = buyPannel->findChild<QGroupBox*>("bodyBox");
            bodyUI->findChild<QScrollArea*>("propList")->setWidget(getPropUI());
        }

        buyPannel->show();
    }
    else
    {
        //距离过远
        QGroupBox *msg = new QGroupBox(gameWindow->ui->gamePannel);
        QLabel *failLab = new QLabel(msg);
        failLab->setText("距离太远,无法购买");
        QPushButton *btn = new QPushButton(msg);
        btn->setText("确定");
        connect(btn,&QPushButton::clicked,[=](){
            msg->close();
            delete failLab;
            delete btn;
            delete msg;
        });

        failLab->move(0,0);
        btn->move(0,20);
        msg->move(gameWindow->ui->gamePannel->width()/2-msg->width()/2,gameWindow->ui->gamePannel->height()/2-msg->height()/2);
        msg->show();
    }

    return true;
}
