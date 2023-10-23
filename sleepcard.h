#ifndef SLEEPCARD_H
#define SLEEPCARD_H

#include "prop.h"
#include <Affect.h>

class SleepCard:public Prop
{
    Q_OBJECT
private:
    Affect *affect;
public:
    SleepCard();
    bool use(Player *targetPlayer,int currentRound);
    bool showRequestVarUI();
};

#endif // SLEEPCARD_H
