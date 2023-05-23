#ifndef UNDEFINEDMAP_H
#define UNDEFINEDMAP_H
#include "AbstractMap.h"
#include <QObject>
#include "mapfactory.h"

class UndefinedMap:public AbstractMap
{
public:
    friend class MapFactory;
private:
    UndefinedMap(int x,int y);
    bool init();
};
#endif // UNDEFINEDMAP_H
