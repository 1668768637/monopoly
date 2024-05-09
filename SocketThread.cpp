#include "SocketThread.h"
#include <JsonTool.h>
#include <QDateTime>
#include <QFile>

QJsonObject* SocketThread::heartPackageJson = []{
    QJsonObject *json = new QJsonObject();
    json->insert("type","heartPackage");
    json->insert("data",QJsonValue::Null);
    return json;
}();

SocketThread::SocketThread(monopolyGame *game, QString IP, int HPTInertval)
    :QThread(nullptr),socket(nullptr),runThread(nullptr),heartPackageTimer(nullptr),tryConnectTimer(nullptr),heartPackageTimerInterval(HPTInertval)
{
    this->game = game;
    this->IP = IP;
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
        socket->connectToHost(IP, 8000);
        socketMutex.unlock();
        qDebug()<<"try connect";
    });

    socketMutex.lock();
    this->socket = new QTcpSocket();
    socket->connectToHost(IP, 8000);
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

    //收到新消息
    QObject::connect(socket,&QTcpSocket::readyRead,this,&SocketThread::receivedNewMsg);
    exec();
}


void SocketThread::sendMessage(QJsonObject *json)
{
    emit this->sendMessageSignal(json);
}

void SocketThread::setResponseFunction(QString type, void (*func)(monopolyGame *, QJsonObject *))
{
    this->responseToFunctions.insert(type,func);
}

QString SocketThread::getIP() const
{
    return IP;
}

void SocketThread::setIP(const QString &newIP)
{
    IP = newIP;
}


void SocketThread::sendMessageSlot(QJsonObject *msg)
{
    //附加发送时间
    msg->insert("sendDate",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    // 在数据前面加入文件大小信息
    QByteArray msgArray = JsonTool::jsonToString(msg).toStdString().data();
    QByteArray sizeData;
    QDataStream sizeStream(&sizeData, QIODevice::WriteOnly);
    sizeStream << qint64(msgArray.size());

    socketMutex.lock();
    this->socket->write(sizeData);
    this->socket->write(msgArray.data());
    socket->flush();
    socketMutex.unlock();

    //this->socket->waitForReadyRead();
    //释放除心跳包之外的msg内存
    if(msg->value("type").toString() != "heartPackage")
    {
        delete msg;
    }
}

void SocketThread::receivedNewMsg()
{
    qint64 msgSize;
    QDataStream dataStream(this->socket);
    dataStream >> msgSize;

    // 等待消息数据
    QByteArray msgByteArray;
    msgSize -= this->socket->bytesAvailable();
    msgByteArray.append(this->socket->readAll());
    while (msgSize > 0) {
        this->socket->waitForReadyRead();
        msgSize -= this->socket->bytesAvailable();
        msgByteArray.append(this->socket->readAll());
    }

    //根据不同的type对回复做出不同的解析
    //获取回复
    QJsonObject *response = new QJsonObject(JsonTool::stringToJson(msgByteArray));
    qDebug()<<*response;

    if(responseToFunctions.contains(response->value("type").toString()))
    {
        //找到了相应type的回复
        void (*function)(monopolyGame*,QJsonObject*) = responseToFunctions.value(response->value("type").toString());
        //执行相应的处理函数
        function(game,response);
    }
    else
    {
        qDebug()<<"未找到对应回复函数";
    }

}
