#ifndef SENCE_H
#define SENCE_H
#include "AbstractMap.h"
#include "QObject"
class Sence:public AbstractMap
{
    Q_OBJECT
public:
    Sence();
    bool init();
};
#endif // SENCE_H
