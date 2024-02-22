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

class monopolyGame;
class SocketThread : public QThread
{
    Q_OBJECT

public:
    explicit SocketThread(monopolyGame *game,int HPTInertval = 1000);
    explicit SocketThread(int HPTInertval);
    ~SocketThread();
    void run() override;
    void sendMessage(QJsonObject *json);

    void setResponseFunction(QString type,void (*)(monopolyGame* game,QJsonObject *msg));

private:
    monopolyGame *game;
    QTcpSocket* socket;
    QMutex socketMutex;
    QWaitCondition condition;
    QThread* runThread;
    QTimer* heartPackageTimer;
    QTimer* tryConnectTimer;
    QQueue<QJsonObject*> taskQueue;
    int heartPackageTimerInterval;
    QHash<QString , void (*)(monopolyGame* game,QJsonObject *msg) > responseToFunctions;

    static QJsonObject* heartPackageJson;


private slots:
    void sendMessageSlot(QJsonObject *msg);
    void receivedNewMsg();
signals:
    //发送数据兵进行解析，向UI线程发出相应的信号
    void sendMessageSignal(QJsonObject *msg);
    void connectSuccessful();
    void connectBroken();
    void RunThread(QThread* thread);

    //向UI线程发出的信号
    void setCurrentDelay(int ms);//最新的延迟
    void logout();
    void SignInSuccess(QString msg);
    void SignInError(QString msg);
};


#endif // SOCKETTHREAD_H
