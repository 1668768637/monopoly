#include "log.h"
#include "qjsonobject.h"
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

    // 在构造函数中手动连接信号和槽
//    connect(ui->btnLogin, &QPushButton::clicked, this, &log::on_btnLogin_clicked);

}

log::~log()
{
    delete ui;
}


void log::on_btnLogin_clicked()
{
    QString ID = ui->ID->text();
    QString password = ui->password->text();
    QJsonObject *request = new QJsonObject();
    request->insert("type","login");
    request->insert("ID",ID);
    request->insert("password",password);
    emit login(request);
}

