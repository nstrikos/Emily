#include "playerimpl.h"

PlayerImpl::PlayerImpl()
{
    qDebug() << "PlayerImpl constructor called";
    m_indexHandler = NULL;
    buffer = NULL;
    qMediaPlayer = new QMediaPlayer();
    connect(qMediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(play()));

    //default value
    qMediaPlayer->setPlaybackRate(1.0);
    m_count = 0;
}

PlayerImpl::~PlayerImpl()
{
    qDebug() << "PlayerImpl destructor called";
    delete qMediaPlayer;
    clearCreatedBuffers();
    clearPlayedBuffers();
    //Maybe it is not good idea to delete buffer, maybe download manager still uses it
    if (buffer != NULL)
    {
        delete buffer;
        buffer = NULL;
        m_count++;
    }
    qDebug() << "Player: Delete buffer number: " << m_count;
}

void PlayerImpl::setIndexHandler(PlayerIface *indexHandler)
{
    m_indexHandler = indexHandler;
}

void PlayerImpl::playFile(QBuffer *buffer, QString index)
{
    createdFileIndexes << index;
    createdBuffers.append(buffer);
    play();
}

void PlayerImpl::play()
{
    //If player is in stopped state
    //take one buffer from createdBuffers
    //append it to playedBuffers
    //play it
    //delete played buffers
    //Finally send index to nvda
    //When finished playing the function will be called again


    if (qMediaPlayer->state() == QMediaPlayer::StoppedState)
    {
        if (!createdBuffers.isEmpty())
        {
            buffer = createdBuffers.takeFirst();
            playedBuffers.append(buffer);
            qMediaPlayer->setMedia(QMediaContent(), buffer);
            buffer->open(QIODevice::ReadOnly);
            qMediaPlayer->play();
            clearPlayedBuffers();
            sendIndexToNVDA();
        }
    }
}

void PlayerImpl::sendIndexToNVDA()
{
    if (!createdFileIndexes.isEmpty())
    {
        QString indexToSend = createdFileIndexes.takeFirst();
        if (indexToSend != "")
        {
            if (m_indexHandler != NULL)
                m_indexHandler->sendIndex(indexToSend);
            else
                qDebug() << "Player: I have indexes to send, but I have nowhere to send them";
        }
    }
}

void PlayerImpl::clearPlayedBuffers()
{
    //Delete all buffers except the buffer that is in qMediaPlayer
    //If this buffer is deleted application will crash
    for (int k = 0; k < playedBuffers.size(); k++)
    {
        QBuffer *tempBuffer = playedBuffers.first();
        if ( tempBuffer != buffer)
        {
            delete tempBuffer;
            m_count++;
            if (!playedBuffers.isEmpty())
                playedBuffers.takeFirst();
        }
    }
}

void PlayerImpl::clearCreatedBuffers()
{
    while (!createdBuffers.isEmpty())
    {
        QBuffer *tempBuffer = createdBuffers.takeFirst();
        delete tempBuffer;
        m_count++;
    }
}

void PlayerImpl::setRate(QString rateString)
{
    float rate = rateString.toFloat();
    rate = (0.01 * rate) + 0.5;
    qMediaPlayer->setPlaybackRate(rate);
    qDebug() << "Player: Set rate to: " << rate;
}

void PlayerImpl::stop()
{
    clearCreatedBuffers();
    createdFileIndexes.clear();
    clearPlayedBuffers();
    qMediaPlayer->stop();
}

void PlayerImpl::resume()
{
    qMediaPlayer->play();
}

void PlayerImpl::pause()
{
    qMediaPlayer->pause();
}
