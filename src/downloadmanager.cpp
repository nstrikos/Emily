#include "downloadmanager.h"

DownloadManager::DownloadManager(){
    reply = NULL;
    downloading = false;
    clipBoardEnabled  = false;
    count = 0;
    connect(this, SIGNAL(finished(QString, QString)), this, SLOT(processLists()));
//    timer = new QTimer();
//    connect(timer, SIGNAL(timeout()), this, SLOT(processLists()));
//    timer->start(50);
}

DownloadManager::~DownloadManager()
{
    if(QFile::exists(filename))
        QFile::remove(filename);
}

void DownloadManager::processLists()
{
    //if (!downloading && clipBoardEnabled)
    if (!downloading)
    {
        if (!textList.isEmpty())
        {
            QString text = textList.takeFirst();
            QString index = indexList.takeFirst();
            performMaryTTSSpeak(text, index);
        }
    }
}

void DownloadManager::performMaryTTSSpeak(QString text, QString index)
{
    if (!downloading)
    {
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
    emit finished(filename, index);
}

void DownloadManager::setVoice(QString voice)
{
    this->voice = voice;
}

void DownloadManager::addToList(QString text, QString index)
{
        textList << text;
        indexList << index;
}

void DownloadManager::addToClipboardList(QString text)
{
    QStringList tempList = text.split(".");
    foreach (QString tmp, tempList) {

        textList << tmp;
        indexList << "";

    }
}

void DownloadManager::clearLists()
{
    textList.clear();
    indexList.clear();
}

void DownloadManager::setClipBoardEnabled(bool value)
{
    this->clipBoardEnabled = value;
}
