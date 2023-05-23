#ifndef SLEEPYCARD_H
#define SLEEPYCARD_H

#include "sleepy.h"
#include "prop.h"

class SleepCard:public Prop
{
    Q_OBJECT
private:
    Affect *affect;
public:
    SleepCard();
    bool use(Player *targetPlayer,int currentRound);
};

#endif // SLEEPYCARD_H
