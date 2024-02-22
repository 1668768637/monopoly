#include "sleepcard.h"
#include "sleepy.h"
#include "player.h"
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAbstractItemDelegate>
#include "monopolygame.h"

SleepCard::SleepCard():Prop(PropType::control)
{
    affect = new Sleepy(-1);
    this->setPixmap(QPixmap(":/res/img/sleepCard.png"));
    this->setToolTip("对目标使用，使其两回合无法行动");
    setPrice(500);
}

bool SleepCard::use(Player *targetPlayer, int currentRound)
{
    if(targetPlayer == nullptr)
    {
        return false;
    }
    else
    {
        affect->beginRund = currentRound;
        targetPlayer->getStateController()->addAffect(affect);
        return true;
    }
}

bool SleepCard::showRequestVarUI()
{
    if(gameWindow->ui->gamePannel->findChild<QGroupBox*>("knapsackUI")->findChild<QGroupBox*>("useWidget") == nullptr)
    {
        int UIwidth = 200,UIheight = 150;
        QGroupBox *knapsackUI = gameWindow->ui->gamePannel->findChild<QGroupBox*>("knapsackUI");
        QWidget *useWidget = new QWidget(knapsackUI);
        useWidget->setObjectName("useWidget");
        useWidget->setGeometry(knapsackUI->width()/2-UIwidth/2,knapsackUI->height()/2-UIheight/2,UIwidth,UIheight);

        //使用冬眠卡所需的条件
        QGroupBox *requestVar = new QGroupBox(useWidget);
        requestVar->setGeometry(0,0,useWidget->width(),50);
        QComboBox *playerBox = new QComboBox(requestVar);
        for(int i = 0;i < gameWindow->playerList.length();i++)
        {
            if(gameWindow->runningPlayer->getName() != gameWindow->playerList.at(i)->getName())
            {
                playerBox->addItem(gameWindow->playerList.at(i)->getName());
            }
        }
        playerBox->setGeometry(0,0,useWidget->width(),50);
        for(int i = 0;i < playerBox->count();i++)
        {
            playerBox->model()->setData(playerBox->model()->index(i,0),QSize(100,50),Qt::SizeHintRole);
        }
        //确定和取消按钮
        QHBoxLayout *btns = new QHBoxLayout(useWidget);
        QPushButton *confirmBtn = new QPushButton();
        confirmBtn->setText("确定");
        connect(confirmBtn,&QPushButton::clicked,[=](){
            Player *targetPlayer;
            for(int i = 0;i < gameWindow->playerList.length();i++)
            {
                if(gameWindow->playerList.at(i)->getName() == playerBox->currentText())
                {
                    targetPlayer = gameWindow->playerList.at(i);
                }
            }
            use(targetPlayer,gameWindow->roundController.getRound());
            gameWindow->runningPlayer->getKnapsack()->removeProp(this);
            emit used();

            useWidget->close();
            delete useWidget;
        });
        QPushButton *cancleBtn = new QPushButton();
        cancleBtn->setText("取消");
        connect(cancleBtn,&QPushButton::clicked,[=](){
            useWidget->close();
            delete useWidget;
        });

        btns->addWidget(confirmBtn,Qt::AlignCenter);
        btns->addWidget(cancleBtn,Qt::AlignCenter);
        btns->setGeometry(QRect(0,requestVar->height(),useWidget->width(),50));


        useWidget->show();
    }
    else
    {
        ;
    }

    return true;
}

