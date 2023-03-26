#ifndef PROP_H
#define PROP_H
#include "GameObject.h"
#include "QObject"
class Prop:public GameObjcet
{
    Q_OBJECT
protected:
    Prop() = default;
};
#endif // PROP_H
