#include "player.h"

static const int TotalBytes = 50 * 1024 * 1024;
static const int PayloadSize = 64 * 1024; // 64 KB

Player::Player()
{
    //Create download manager object
    downloadManager = new DownloadManager(textList, indexList);
    connect(downloadManager, SIGNAL(finished(QString, QString, QString)), this, SLOT(playFile(QString, QString, QString)));
    voice = googleVoice;
    downloadManager->setVoice(voice);

    //Create playlist object
    createPlayListModel();
    connect(&playlist, SIGNAL(currentIndexChanged(int)), this,  SLOT(informNVDA()));

    //Set rate
    //rate = 1.0;
    //qMediaPlayer.setPlaybackRate(rate);

    //Create, connect server objects and set timers
    connect(&nvdaTextServer, SIGNAL(newConnection()),
            this, SLOT(nvdaTextServerAcceptConnection()));
    connect(&nvdaCommandServer, SIGNAL(newConnection()),
            this, SLOT(nvdaCommandServerAcceptConnection()));
    connect(&nvdaIndexServer, SIGNAL(newConnection()),
            this, SLOT(nvdaIndexServerAcceptConnection()));

    nvdaTextServer.listen(QHostAddress::LocalHost, 57116);
    nvdaCommandServer.listen(QHostAddress::LocalHost, 57117);
    nvdaIndexServer.listen(QHostAddress::LocalHost, 57118);

    nvdaIndexServerConnection = NULL;
}

Player::~Player()
{
    //Close connections
    nvdaTextServer.close();
    nvdaCommandServer.close();
    nvdaIndexServer.close();

    //Free memory
    if (downloadManager != NULL)
    {
        delete downloadManager;
        downloadManager = NULL;
    }

    //Clear all files
    while (!playedFiles.isEmpty())
    {
        QString file = playedFiles.takeFirst();
        QFile::remove(file);
    }

    clearFiles();
}

void Player::createPlayListModel()
{
    qMediaPlayer.setPlaylist(&playlist);
    if (!qMediaPlayer.isAvailable())
    {
        QMessageBox msgBox;
        msgBox.setText( QObject::tr("The QMediaPlayer object does not have a valid service.\n"\
                                    "Please check the media service plugins are installed.") );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
    }
}

void Player::playFile(QString filename, QString text, QString index)
{
    this->index = index;
    this->spokenText = text;
    fileList << filename;
    createdFiles << filename;
    spokenIndex << index;
    informNVDA();
}

void Player::addToPlaylist(const QString& filename)
{
    QFileInfo fileInfo(filename);
    if (fileInfo.exists())
    {
        QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
        playlist.addMedia(url);
    }
}

void Player::setVoice(QString voice)
{
    this->voice = voice;
    //    if (voice == emilyVoice)
    //        rate = 0.8;
    //    else
    //rate = 1.0;
    downloadManager->setVoice(voice);
    //qMediaPlayer.setPlaybackRate(rate);
}

void Player::increaseRate()
{
    //rate += 0.05;
    //qMediaPlayer.setPlaybackRate(rate);
}

void Player::decreaseRate()
{
    //rate -= 0.05;
    //if ( rate <= 0)
    //    rate = 0.05;
    //qMediaPlayer.setPlaybackRate(rate);
}

void Player::setRate(QString rateString)
{
    float rate = rateString.toFloat();
    rate = (0.01 * rate) + 0.5;
    qMediaPlayer.setPlaybackRate(rate);
}

void Player::informNVDA()
{
    if (playlist.currentIndex() == -1)
    {
        playlist.clear();
        while (!playedFiles.isEmpty())
        {
            QString file = playedFiles.takeFirst();
            QFile::remove(file);
        }
        if (!fileList.isEmpty())
        {
            QString filename = fileList.takeFirst();
            playedFiles << filename;
            addToPlaylist(filename);
            qMediaPlayer.play();
        }

        if (nvdaIndexServerConnection != NULL)
        {
            if (!spokenIndex.isEmpty())
            {
                QString index = spokenIndex.takeFirst();
                if (index != "")
                {
                    QByteArray textTemp = index.toUtf8() ;
                    nvdaIndexServerConnection->write(textTemp);
                }
            }
        }

        downloadManager->processLists();

    }
}

void Player::nvdaTextServerAcceptConnection()
{
    nvdaTextServerConnection = nvdaTextServer.nextPendingConnection();
    connect(nvdaTextServerConnection, SIGNAL(readyRead()), this, SLOT(updatenvdaTextServerProgress()));
}

void Player::nvdaCommandServerAcceptConnection()
{
    nvdaCommandServerConnection = nvdaCommandServer.nextPendingConnection();
    connect(nvdaCommandServerConnection, SIGNAL(readyRead()),
            this, SLOT(updatenvdaCommandServerProgress()));
}

void Player::nvdaIndexServerAcceptConnection()
{
    nvdaIndexServerConnection = nvdaIndexServer.nextPendingConnection();
}

void Player::updatenvdaTextServerProgress()
{

    //This code handles incoming text from nvda

    QString result(nvdaTextServerConnection->readAll());
    if (result != "")
    {
        if (result.contains(nvdaIndex))
        {
            bool done = false;
            while (!done)
            {
                int indexPosition = result.indexOf(nvdaIndex);
                QString firstPart = result.left(indexPosition);
                if (firstPart != "")
                    downloadManager->addToList(firstPart, "");
                result = result.right(result.size() - indexPosition);
                int nextIndexPosition = result.indexOf("#");
                QString indexString = result.left(nextIndexPosition);
                indexString = indexString.replace(nvdaIndex, "");
                QString leftover = result.right(result.size() - nextIndexPosition - 1);
                if (!leftover.contains(nvdaIndex))
                {
                    downloadManager->addToList(leftover, indexString);
                    done = true;
                }
                else
                {
                    //Do the same again
                    int d = leftover.indexOf(nvdaIndex);
                    QString line = leftover.left(d);
                    downloadManager->addToList(line, indexString);
                    result = leftover.right(leftover.size() - d);
                }
            }
        }
        else
            downloadManager->addToList(result, "");

        //Finally
        informNVDA();
    }
}

void Player::updatenvdaCommandServerProgress()
{

    QString result(nvdaCommandServerConnection->readAll());
    qDebug() << result;
    if (result != "")
    {
        if (result.contains("Cancel"))
            stop();
        else if (result.contains("Pause"))
            pause();
        else if (result.contains("Start"))
            resume();
        else if (result.contains("Rate "))
        {
            result.replace("Rate ", "");
            setRate(result);
        }
    }
}

void Player::speakClipBoardText(QString text)
{
    clearFiles();
    qMediaPlayer.stop();
    if (!qMediaPlayer.playlist()->isEmpty())
        qMediaPlayer.playlist()->clear();
    downloadManager->clearLists();
    fileList.clear();
    indexList.clear();
    downloadManager->addToClipboardList(text);
    informNVDA();
}

void Player::stop()
{
    //It is necessary to keep this order
    //otherwise the player will freeze
    qMediaPlayer.stop();
    if (!qMediaPlayer.playlist()->isEmpty())
        qMediaPlayer.playlist()->clear();
    downloadManager->clearLists();
    downloadManager->cancelDownload();
    fileList.clear();
    indexList.clear();
    clearFiles();
}

void Player::pause()
{
    qMediaPlayer.pause();
}

void Player::resume()
{
    qMediaPlayer.play();
}

void Player::clearFiles()
{
    while (!createdFiles.isEmpty())
    {
        QString file = createdFiles.takeFirst();
        QFile::remove(file);
    }
}
