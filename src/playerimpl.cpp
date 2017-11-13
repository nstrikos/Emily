/*
 * Explanation: When we want to play a buffer, we call addPlayList
 * to add the buffer with its index.
 * We also implement stop, resume, pause and setRate
*/

#include "playerimpl.h"

PlayerImpl::PlayerImpl()
{
    m_indexHandler = NULL;
    m_buffer = NULL;
    m_delCount = 0;
    m_mediaPlayer = new QMediaPlayer();
    m_mediaPlayer->setPlaybackRate(1.0);
    connect(m_mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(play()));
}

void PlayerImpl::addPlaylist(QBuffer *buffer, QString index)
{
    m_createdIndexes << index;
    m_createdBuffers.append(buffer);
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


    if (m_mediaPlayer->state() == QMediaPlayer::StoppedState)
    {
        if (!m_createdBuffers.isEmpty())
        {
            m_buffer = m_createdBuffers.takeFirst();
            m_playedBuffers.append(m_buffer);
            m_mediaPlayer->setMedia(QMediaContent(), m_buffer);
            m_buffer->open(QIODevice::ReadOnly);
            m_mediaPlayer->play();
            clearPlayedBuffers();
            sendIndexToNVDA();
        }
    }
}

void PlayerImpl::clearPlayedBuffers()
{
    // Delete all buffers except the buffer that is in m_mediaPlayer
    // If this buffer is deleted m_mediaPlayer will crash
    // causing application to crash
    for (int k = 0; k < m_playedBuffers.size(); k++)
    {
        QBuffer *tempBuffer = m_playedBuffers.first();
        if ( tempBuffer != m_buffer)
        {
            if ( tempBuffer != NULL)
            {
                delete tempBuffer;
                m_delCount++;
            }
            m_playedBuffers.removeFirst();
        }
    }
}

void PlayerImpl::sendIndexToNVDA()
{
    if (!m_createdIndexes.isEmpty())
    {
        QString indexToSend = m_createdIndexes.takeFirst();
        if (indexToSend != "")
        {
            if (m_indexHandler != NULL)
                m_indexHandler->sendIndex(indexToSend);
        }
    }
}

void PlayerImpl::stop()
{
    clearCreatedBuffers();
    m_createdIndexes.clear();
    clearPlayedBuffers();
    m_mediaPlayer->stop();
}

void PlayerImpl::clearCreatedBuffers()
{
    while (!m_createdBuffers.isEmpty())
    {
        QBuffer *tempBuffer = m_createdBuffers.takeFirst();
        delete tempBuffer;
        m_delCount++;
    }
}

void PlayerImpl::resume()
{
    m_mediaPlayer->play();
}

void PlayerImpl::pause()
{
    m_mediaPlayer->pause();
}

void PlayerImpl::setRate(QString rateString)
{
    float rate = rateString.toFloat();
    rate = (0.01 * rate) + 0.5;
    m_mediaPlayer->setPlaybackRate(rate);
}

void PlayerImpl::setIndexHandler(PlayerIface *indexHandler)
{
    m_indexHandler = indexHandler;
}

PlayerImpl::~PlayerImpl()
{
    qDebug() << "PlayerImpl destructor called";
    delete m_mediaPlayer;
    clearCreatedBuffers();
    clearPlayedBuffers();

    //We delete the last buffer, this buffer is in mediaPlayer.setMedia
    //If we delete this buffer, the application will crash
    //So we first delete mediaPlayer and then we delete m_buffer
    if (m_buffer != NULL)
    {
        delete m_buffer;
        m_buffer = NULL;
        m_delCount++;
    }
    qDebug() << "PlayerImpl: Number of deleted buffers: " << m_delCount;
    qDebug() << "PlayerImpl destructor completed";
}

