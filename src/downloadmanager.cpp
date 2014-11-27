#include "downloadmanager.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QDir>
#include <QTextCodec>

DownloadManager::DownloadManager(QStringList textList, QStringList indexList){
    reply = NULL;
    downloading = false;
    count = 0;
    this->indexList = indexList;
    this->textList = textList;
    //connect(this, SIGNAL(finished(QString)), this, SLOT(processLists()));
}

DownloadManager::~DownloadManager()
{
    if(QFile::exists(filename))
        QFile::remove(filename);
}

void DownloadManager::processLists()
{
    if (!downloading)
    {
        //qDebug() << "Text list not empty";
        if (!textList.isEmpty())
        {
            QString text = textList.takeFirst();
            QString index = indexList.takeFirst();
            performMaryTTSSpeak(text, index);
        }
        else
        {
            //qDebug() << "Text list is empty";
        }

    }
}

void DownloadManager::performMaryTTSSpeak(QString text, QString index)
{
    if (!downloading)
    {
        //qDebug() << "Try to process:" << text;
        downloading = true;
        this->index = index;
        this->textToSpeak = text;
        filename = QDir::tempPath() + "/emily" + QString::number(count++) + ".wav";

        QString finalCommand = "http://localhost:59125/process?INPUT_TEXT=";
        QString command1 = "&INPUT_TYPE=TEXT&OUTPUT_TYPE=AUDIO&AUDIO=AU_FILE&";
        QString command2 = voice;
        //text = text.left(100);
        //finalCommand = "http://translate.google.com/translate_tts?ie=UTF-8&tl=el&q=" + text;
        finalCommand = finalCommand + text + command1 + command2;

        file = new QFile(filename);
        if (!this->file->open(QIODevice::WriteOnly))
        {
            delete this->file;
            this->file = 0;
            qDebug() << "Return without starting request";
            return;
        }
        // schedule the request
        httpRequestAborted = false;
        startRequest(finalCommand);
    }
}

void DownloadManager::startRequest(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),  this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}

void DownloadManager::httpFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        reply = NULL;
        return;
    }
    file->flush();
    file->close();

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
    finishRequest();
}

void DownloadManager::cancelDownload()
{
    if ( reply != NULL)
    {
        httpRequestAborted = true;
        reply->abort();
        downloading = false;
        emit cancelled();
    }
}

void DownloadManager::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void DownloadManager::finishRequest()
{
    downloading = false;
    emit finished(filename, textToSpeak, index);
}

void DownloadManager::setVoice(QString voice)
{
    this->voice = voice;
}

void DownloadManager::addToList(QString text, QString index)
{
    textList << text;
    indexList << index;
    //processLists();
}

void DownloadManager::clearLists()
{
    textList.clear();
    indexList.clear();
}
