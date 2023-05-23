#ifndef PROP_H
#define PROP_H
#include "GameObject.h"
#include <QMouseEvent>
#include "QObject"
class Player;

enum class PropType{control=1010,function,unShow,other};

class Prop:public GameObjcet
{
    Q_OBJECT
public:
    PropType type;


    virtual bool use(Player* targetPlayer,int currentRound) = 0;

    void mousePressEvent(QMouseEvent* ev)
    {
        if (ev != nullptr && ev->button() == Qt::LeftButton)
        {
            emit clicked();    //emit signal
        }
    }

    void enterEvent(QEvent*)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    void leaveEvent(QEvent*)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
signals:
    void clicked();
protected:
    Prop(PropType t);
};
#endif // PROP_H
