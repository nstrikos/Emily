/*
 * Class : PlayerImpl
 * Role : My responsibility is to implement Player class
 * Collaborator : When a wav file is played I send its index to m_indexHandler
 * which probably is a textMediator object
*/

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
    void addPlaylist(QBuffer* buffer, QString index);
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
    QMediaPlayer *m_mediaPlayer;
    QVector<QBuffer*> m_createdBuffers;
    QVector<QBuffer*> m_playedBuffers;
    QBuffer *m_buffer;
    QStringList m_createdIndexes;

    PlayerIface *m_indexHandler;
};

#endif // PLAYERIMPL_H
