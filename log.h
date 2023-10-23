#ifndef LOG_H
#define LOG_H

#include <QWidget>

namespace Ui {
class logWidget;
}

class log : public QWidget
{
    Q_OBJECT

public:
    explicit log(QWidget *parent = nullptr);
    ~log();

private slots:
    void on_btnLogin_clicked();
signals:
    void login(QJsonObject *request);

private:
    Ui::logWidget *ui;
};

#endif // LOG_H
