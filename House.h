#ifndef HOUSE_H
#define HOUSE_H
#include "OperablePlace.h"
#include "QObject"
class House:public OperablePlace
{
    Q_OBJECT
public:
    House();
    bool option();
    bool init();
};
#endif // HOUSE_H
