#ifndef PROPFACTORY_H
#define PROPFACTORY_H

#include "Prop.h"


class PropFactory{
public:
    static Prop* createProp(int propCode);
};

#endif // PROPFACTORY_H
