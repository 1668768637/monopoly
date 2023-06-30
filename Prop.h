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
    float price;

    virtual bool use(Player* targetPlayer,int currentRound) = 0;
    virtual bool showRequestVarUI() = 0;

    void mousePressEvent(QMouseEvent* ev)
    {
        if (ev != nullptr && ev->button() == Qt::LeftButton)
        {
            emit clicked();    //emit signal
        }
    }

    void enterEvent(QEnterEvent*)
    {
        setCursor(QCursor(Qt::PointingHandCursor));
    }
    void leaveEvent(QEvent*)
    {
        setCursor(QCursor(Qt::ArrowCursor));
    }
signals:
    void clicked();
    void used();
protected:
    Prop(PropType t);
};
#endif // PROP_H
