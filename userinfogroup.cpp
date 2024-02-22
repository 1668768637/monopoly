#include "userinfogroup.h"
#include "ui_userinfogroup.h"

userinfoGroup::userinfoGroup(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::userinfoGroup)
{
    ui->setupUi(this);
}

userinfoGroup::~userinfoGroup()
{
    delete ui;
}
