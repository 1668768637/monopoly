#ifndef PLAYER_H
#define PLAYER_H
#include"abstractplayer.h"

class Player:public AbstractOlayer
{
    Q_OBJECT
private:
    int id;
    QString name;
    //背包
public:
    Player();
    bool run();
    bool init();
};
#endif // PLAYER_H

