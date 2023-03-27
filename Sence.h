#ifndef SENCE_H
#define SENCE_H
#include "AbstractMap.h"
#include "QObject"
#include "mapfactory.h"

class Sence:public AbstractMap
{
    Q_OBJECT
public:
    friend class MapFactory;
    bool init();
private:
    Sence();
};
#endif // SENCE_H
