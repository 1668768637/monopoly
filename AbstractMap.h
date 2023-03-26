#ifndef ABSTRACTMAP_H
#define ABSTRACTMAP_H
#include "GameObject.h"
#include "QObject"

class AbstractMap:public GameObjcet
{
    Q_OBJECT
public:
    AbstractMap();
    virtual bool init() = 0;
};
#endif // ABSTRACTMAP_H
