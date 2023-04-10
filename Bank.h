#ifndef BANK_H
#define BANK_H
#include "OperablePlace.h"
#include "QObject"
class Bank:public OperablePlace
{
    Q_OBJECT
public:
    Bank();
    bool option();
};
#endif // BANK_H
