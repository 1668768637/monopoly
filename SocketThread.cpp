#include "SocketThread.h"
#include <JsonTool.h>
#include <QDateTime>

QJsonObject* SocketThread::heartPackageJson = []{
    QJsonObject *json = new QJsonObject();
    json->insert("type","heartPackage");
    json->insert("data",QJsonValue::Null);
    return json;
}();

QHash<QString,int> SocketThread::jsonType = []{
    QHash<QString,int> hash;
    //增加通信类型
    hash.insert("heartPackage",1);
    hash.insert("login",2);
    return hash;
}();

SocketThread::SocketThread(QObject *parent,int HPTInertval)
    :QThread(parent),socket(nullptr),runThread(nullptr),heartPackageTimer(nullptr),tryConnectTimer(nullptr),heartPackageTimerInterval(HPTInertval)
{
}

SocketThread::SocketThread(int HPTInertval):SocketThread(nullptr,HPTInertval)
{
}

SocketThread::~SocketThread()
{
    this->quit();
}

void SocketThread::run()
{
    emit RunThread(currentThread());

    //初始化心跳包定时器
    heartPackageTimer = new QTimer();
    heartPackageTimer->setInterval(heartPackageTimerInterval);//间隔一定时间发送心跳包
    QObject::connect(heartPackageTimer,&QTimer::timeout,this,[=](){
        emit sendMessageSignal(SocketThread::heartPackageJson);
    });

    //初始化重连信号定时器
    tryConnectTimer = new QTimer();
    tryConnectTimer->setInterval(3000);
    QObject::connect(tryConnectTimer,&QTimer::timeout,this,[&](){
        socketMutex.lock();
        socket->abort();
        socket->connectToHost("127.0.0.1", 8000);
        socketMutex.unlock();
        qDebug()<<"try connect";
    });

    socketMutex.lock();
    this->socket = new QTcpSocket();
    socket->connectToHost("127.0.0.1", 8000);
    socketMutex.unlock();


    //socket连接成功
    QObject::connect(socket,&QTcpSocket::connected,this,[=](){
        qDebug()<<"链接成功";
        emit connectSuccessful();
        //链接发送信息槽函数
        QObject::connect(this,&SocketThread::sendMessageSignal,this,&SocketThread::sendMessageSlot);

        if(!heartPackageTimer->isActive())heartPackageTimer->start();
        tryConnectTimer->stop();
    });

    //socket链接发生错误
    QObject::connect(socket,&QTcpSocket::errorOccurred,this,[=](QAbstractSocket::SocketError socketError){
        if (socketError == QAbstractSocket::SocketTimeoutError) {
            // 处理连接超时的情况
            qDebug() << "Connection timeout!";
        } else {
            // 处理其他套接字错误
            qDebug() << "Socket error: " << socket->errorString();
        }

        heartPackageTimer->stop();
        if(!tryConnectTimer->isActive())tryConnectTimer->start();
    });

    //socket断开连接
    QObject::connect(socket,&QTcpSocket::disconnected,this,[&](){
        QObject::disconnect(this,&SocketThread::sendMessageSignal,this,&SocketThread::sendMessageSlot);
        heartPackageTimer->stop();
        if(!tryConnectTimer->isActive())tryConnectTimer->start();
        emit connectBroken();
    });

    exec();
}


void SocketThread::sendMessage(QJsonObject *json)
{
    emit this->sendMessageSignal(json);
}

void SocketThread::heartPackageAcrion()
{

}



void SocketThread::sendMessageSlot(QJsonObject *msg)
{
    //附加发送时间
    msg->insert("date",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    socketMutex.lock();
    this->socket->write(JsonTool::jsonToString(msg).toStdString().data());
    socketMutex.unlock();

    //根据不同的type对回复做出不同的解析
    //获取回复
    this->socket->waitForReadyRead();
    QJsonObject response = JsonTool::stringToJson(this->socket->readAll());
    if(jsonType.contains(response.value("type").toString()))
    {
        switch (jsonType.value(response.value("type").toString()))
        {
            //心跳包
        case 1:
        {
            QDateTime sendTime = QDateTime::fromString(msg->value("date").toString(), "yyyy-MM-dd hh:mm:ss.zzz");
            QDateTime respondTime(QDateTime::fromString(response.value("date").toString(), "yyyy-MM-dd hh:mm:ss.zzz"));
            int delay = sendTime.msecsTo(respondTime);
            emit currentDelay(delay);
            break;
        }
            //登录回复
        case 2:
        {
            qDebug()<<response.value("state");
            break;
        }
        }
    }
    //释放msg内存
    if(jsonType.contains(msg->value("type").toString()) != jsonType.value("heartPackage"))
    {
        delete msg;
    }
}
