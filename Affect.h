#ifndef AFFECT_H
#define AFFECT_H
#include "gameobject.h"

class Affect:public GameObjcet
{
    Q_OBJECT
public:
    int beginRund;
    const int lastRounds;
protected:
    Affect(int lastRounds);
};
#endif // AFFECT_H
