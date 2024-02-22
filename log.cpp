#include "log.h"
#include "ui_log.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDebug>
#include <monopolygame.h>

log::log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logWidget)
{
    ui->setupUi(this);

    QRegularExpression  regexPassword("[A-Za-z0-9]*");
    QValidator *validatorPassword = new QRegularExpressionValidator(regexPassword, this);
    ui->password->setValidator(validatorPassword);

    QRegularExpression  regexID("[0-9]*");
    QValidator *validatorID = new QRegularExpressionValidator(regexID, this);
    ui->ID->setValidator(validatorID);


}

log::~log()
{
    delete ui;
}


void log::on_btnLogin_clicked()
{
    QString ID = ui->ID->text();
    QString password = ui->password->text();
    if(ID.isEmpty() || password.isEmpty())
    {
        this->parentWidget()->findChild<QWidget*>("logWidget")->findChild<QLabel*>("logErrorMsgLabel")->setText("用户名或密码为空！！");
    }
    else
    {
        QJsonObject *request = new QJsonObject();
        request->insert("type","login");
        request->insert("ID",ID);
        request->insert("password",password);
        emit login(request);
    }
}

