#include "prop.h"

Prop::Prop(PropType t):type(t)
{
    setFixedSize(QSize(90,140));
    setMinimumSize(QSize(90,140));
}
