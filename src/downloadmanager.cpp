/*
 * Explanation: This class does nothing important, it delegates
 * the requests to DownloadManagerImpl, which implements the functionality
*/

#include "downloadmanager.h"
#include <constants.h>
#include <QBuffer>
#include <QDir>

DownloadManager::DownloadManager()
{
    qDebug() << "Download manager creator called";
    m_impl = new DownloadManagerImpl();
}

void DownloadManager::setWavReceiver(DownloadManagerIface *wavReceiver)
{
    m_impl->setWavReceiver(wavReceiver);
}

void DownloadManager::setVoice(QString voice)
{
    m_impl->setVoice(voice);
}

void DownloadManager::textToSpeech(QString text, QString index)
{
    m_impl->textToSpeech(text, index);
}

void DownloadManager::cancelDownload()
{
    m_impl->cancelDownload();
}

DownloadManager::~DownloadManager()
{
    qDebug() << "Download manager destructor called";
    delete m_impl;
}

