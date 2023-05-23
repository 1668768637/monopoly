#ifndef BUFF_H
#define BUFF_H
#include "Affect.h"

class Buff:public Affect
{
    Q_OBJECT
public:
    Buff(int round);
};

#endif // BUFF_H
