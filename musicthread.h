#ifndef MUSICTHREAD_H
#define MUSICTHREAD_H

#include <QThread>
#include <QtMultimedia/QMediaPlayer>
#include <QMutex>
#include <QWaitCondition>

class MusicThread : public QThread
{
    Q_OBJECT

public:
    explicit MusicThread(QObject *parent = nullptr);
    void run() override;
    void pause();
    void resume();

private:
    QMediaPlayer *player = nullptr;
    QMutex mutex;
    QWaitCondition condition;
    bool paused;

private slots:
};



#endif // MUSICTHREAD_H
