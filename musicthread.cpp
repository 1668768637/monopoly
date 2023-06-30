#include "musicthread.h"
#include <QDebug>
#include <QFile>
#include <QtMultimedia/QAudioOutput>

MusicThread::MusicThread(QObject *parent) : QThread(parent), paused(false)
{
    player = new QMediaPlayer();
    player->setLoops(-1);
    QAudioOutput *output = new QAudioOutput();

    player->setSource(QUrl("qrc:/res/audio/bgm/bgm.mp3"));
    player->setAudioOutput(output);


}

void MusicThread::pause()
{
    mutex.lock();
    paused = true;
    mutex.unlock();
}

void MusicThread::resume()
{
    mutex.lock();
    paused = false;
    mutex.unlock();
    condition.wakeAll();
}


void MusicThread::run()
{
    player->play();

    forever {
        mutex.lock();
        if (paused) {
            condition.wait(&mutex);
        }
        mutex.unlock();

        // 继续播放音乐的逻辑...
    }
}
