#ifndef OPERABLEPLACE_H
#define OPERABLEPLACE_H
#include "AbstractMap.h"
#include "QObject"
class OperablePlace:public AbstractMap
{
    Q_OBJECT
public:
    OperablePlace();
    virtual bool option() = 0;
    bool init() = 0;
};
#endif // OPERABLEPLACE_H
