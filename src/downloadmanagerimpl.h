/*
 * Class : DownloadManagerImpl
 * Role : My responsibility is to implement DownloadManager class
 * Collaborator : When a wav file is created I send it to m_wavReceiver
 * which probably is a textMediator object
*/

#ifndef DOWNLOADMANAGERIMPL_H
#define DOWNLOADMANAGERIMPL_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include "downloadmanageriface.h"

class DownloadManagerImpl : public QObject
{
    Q_OBJECT

public:
    DownloadManagerImpl();
    ~DownloadManagerImpl();
    void setWavReceiver(DownloadManagerIface *wavReceiver);
    void setVoice(QString voice);
    void textToSpeech(QString text, QString index);
    void cancelDownload();

private slots:
    void httpFinished();
    void httpReadyRead();

private:
    void processLists();
    void performTextToWav(QString text, QString index);
    void startRequest(QUrl url);
    void finishRequest();
    void clearLists();

    QBuffer *m_file;
    QNetworkAccessManager m_qnam;
    QNetworkReply *m_reply;
    bool m_httpRequestAborted;
    bool m_downloading;
    QStringList m_textList;
    QStringList m_indexList;
    QString m_index;
    QString m_voice;
    int m_actualBuffers;
    int m_delBuffers;

    DownloadManagerIface *m_wavReceiver;
};

#endif // DOWNLOADMANAGERIMPL_H
