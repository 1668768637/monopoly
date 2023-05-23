#include "bank.h"
Bank::Bank(int x,int y):OperablePlace(x,y)
{
    setObjectName("Bank");
}

bool Bank::option()
{
    return true;
}

