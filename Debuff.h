#ifndef DEBUFF_H
#define DEBUFF_H
#include "Affect.h"

class Debuff:public Affect
{
    Q_OBJECT
public:
    Debuff(int round);
};

#endif // DEBUFF_H
