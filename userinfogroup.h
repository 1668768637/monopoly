#ifndef USERINFOGROUP_H
#define USERINFOGROUP_H

#include <QGroupBox>

namespace Ui {
class userinfoGroup;
}

class userinfoGroup : public QGroupBox
{
    Q_OBJECT

public:
    explicit userinfoGroup(QWidget *parent = nullptr);
    ~userinfoGroup();

private:
    Ui::userinfoGroup *ui;
};

#endif // USERINFOGROUP_H
