#include "propFactory.h"
#include "sleepcard.h"


Prop *PropFactory::createProp(int propCode)
{
    Prop *p = nullptr;
    switch (propCode) {
    case 100:
        p = new SleepCard();
        break;
    }

    return p;
}
