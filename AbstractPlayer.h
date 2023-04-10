#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H
#include "GameObject.h"
#include "QObject"

class AbstractPlayer:public GameObjcet
{
    Q_OBJECT
public:
    AbstractPlayer();
};
#endif // ABSTRACTPLAYER_H
