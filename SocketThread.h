#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTcpSocket>
#include <QTimer>
#include <QQueue>
#include <QJsonObject>
#include <Controller.h>
#include <QHash>


/*This class have these functions:
 * 1. connect to server which saved in files when it's inner thread running
 * 2. if connect successfully,it will send heart-package regularly
 * 3. if failed,it will try to connect to server regularly
 * 4. send messages
 * */
class SocketThread : public QThread
{
    Q_OBJECT

public:
    explicit SocketThread(QObject *parent = nullptr,int HPTInertval = 1000);
    explicit SocketThread(int HPTInertval);
    ~SocketThread();
    void run() override;
    void sendMessage(QJsonObject *json);

private:
    QTcpSocket* socket;
    QMutex socketMutex;
    QWaitCondition condition;
    QThread* runThread;
    QTimer* heartPackageTimer;
    QTimer* tryConnectTimer;
    QQueue<QJsonObject*> taskQueue;
    int heartPackageTimerInterval;

    static QJsonObject* heartPackageJson;
    static QHash<QString , int > jsonType;

    void heartPackageAcrion();


private slots:
    void sendMessageSlot(QJsonObject *msg);
signals:
    void sendMessageSignal(QJsonObject *msg);
    void connectSuccessful();
    void connectBroken();
    void RunThread(QThread* thread);
    void currentDelay(int ms);//最新的延迟
    void loginSuccess();
    void logout();
    void loginError(QString error);
    void SignInSuccess(QString msg);
    void SignInError(QString msg);
};


#endif // SOCKETTHREAD_H
