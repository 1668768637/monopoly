#ifndef ABSTRACTMAP_H
#define ABSTRACTMAP_H
#include "GameObject.h"
#include "QObject"

class AbstractMap:public GameObjcet
{
    Q_OBJECT
public:
    friend class MapFactory;
    virtual bool init() = 0;
    AbstractMap();
private:
};
#endif // ABSTRACTMAP_H
