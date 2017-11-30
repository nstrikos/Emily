/*
 * Class : DownloadManager
 * Role : My responsibility is to convert text to speech,
 * call textToSpeech to convert text to wav
 * Collaborator : When a wav file is created I send it to m_wavReceiver
 * which probably is a textMediator object
*/

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "downloadmanagerimpl.h"

class DownloadManager
{

public:
    DownloadManager();
    ~DownloadManager();
    void setWavReceiver(DownloadManagerIface *wavReceiver);
    void setVoice(QString voice);
    void nextVoice();
    void textToSpeech(QString text, QString index);
    void cancelDownload();

private:
    DownloadManagerImpl *m_impl;
};

#endif // DOWNLOADMANAGER_H
