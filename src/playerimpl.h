#ifndef PLAYERIMPL_H
#define PLAYERIMPL_H

#include "playeriface.h"
#include <QBuffer>
#include <QtMultimedia/QMediaPlayer>

class PlayerImpl : public QObject
{
    Q_OBJECT

public:
    PlayerImpl();
    ~PlayerImpl();
public:
    void setIndexHandler(PlayerIface *indexHandler);
    void playFile(QBuffer* buffer, QString index);
    void setRate(QString rateString);
    void stop();
    void resume();
    void pause();

private slots:
    void play();

private:
    void sendIndexToNVDA();
    void clearPlayedBuffers();
    void clearCreatedBuffers();
    QMediaPlayer *qMediaPlayer;
    QVector<QBuffer*> createdBuffers;
    QVector<QBuffer*> playedBuffers;
    QBuffer *buffer;
    QStringList createdFileIndexes;
    int m_count;

    PlayerIface *m_indexHandler;
};

#endif // PLAYERIMPL_H
