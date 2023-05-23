#ifndef BANK_H
#define BANK_H
#include "OperablePlace.h"
#include "QObject"
class Bank:public OperablePlace
{
    Q_OBJECT
public:
    Bank(int x,int y);
    bool option();
};
#endif // BANK_H
