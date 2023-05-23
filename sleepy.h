#ifndef SLEEPY_H
#define SLEEPY_H
#include "Debuff.h"

//this debuff will make player sleep that cant make actions
class Sleepy:public Debuff
{
    Q_OBJECT
public:
    Sleepy(int begin);
};

#endif // SLEEPY_H
