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
    rate = 1.0;
    qMediaPlayer.setPlaybackRate(rate);

    //Create, connect server objects and set timers
    connect(&nvdaTextServer, SIGNAL(newConnection()),
            this, SLOT(nvdaTextServerAcceptConnection()));
    connect(&tcpServer3, SIGNAL(newConnection()),
            this, SLOT(acceptConnection3()));
    connect(&tcpServer4, SIGNAL(newConnection()),
            this, SLOT(acceptConnection4()));

    nvdaTextServer.listen(QHostAddress::LocalHost, 57116);
    tcpServer3.listen(QHostAddress::LocalHost, 57118);
    tcpServer4.listen(QHostAddress::LocalHost, 57121);


    //why are timers necessary?
    this->index = "0";
    timer4 = new QTimer();
    connect(timer4, SIGNAL(timeout()), this, SLOT(updateIndex()));
    tcpServerConnection4 = NULL;
}

Player::~Player()
{
    //Close connections
    nvdaTextServer.close();
    tcpServer3.close();
    tcpServer4.close();

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
    //addToPlaylist(filename);
    //player.play();
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
    rate = 1.0;
    downloadManager->setVoice(voice);
    qMediaPlayer.setPlaybackRate(rate);
}

void Player::increaseRate()
{
    rate += 0.05;
    qMediaPlayer.setPlaybackRate(rate);
}

void Player::decreaseRate()
{
    rate -= 0.05;
    if ( rate <= 0)
        rate = 0.05;
    qMediaPlayer.setPlaybackRate(rate);
}
void Player::informNVDA()
{
    //qDebug() << "Inform NVDA";
    if (playlist.currentIndex() == -1)
    {
        //qDebug() << "Player is stopped";
        playlist.clear();
        while (!playedFiles.isEmpty())
        {
            QString file = playedFiles.takeFirst();
//            qDebug() << file;
            QFile::remove(file);
        }
        if (!fileList.isEmpty())
        {
            QString filename = fileList.takeFirst();
            playedFiles << filename;
            addToPlaylist(filename);
            qMediaPlayer.play();
        }

        if (tcpServerConnection4 != NULL)
        {
            //if (index != "")
            //{
            if (!spokenIndex.isEmpty())
            {
                QString index = spokenIndex.takeFirst();
                if (index != "")
                {
                    QByteArray textTemp = index.toUtf8() ;
                    tcpServerConnection4->write(textTemp);
                    //qDebug() << "Send index:" << index;
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

void Player::acceptConnection3()
{
    tcpServerConnection3 = tcpServer3.nextPendingConnection();
    //connect(commandTimer, SIGNAL(timeout()), this , SLOT(updateServerProgress3()));
    //commandTimer->start(100);
    connect(tcpServerConnection3, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress3()));
}

void Player::acceptConnection4()
{
    tcpServerConnection4 = tcpServer4.nextPendingConnection();
    //connect(commandTimer, SIGNAL(timeout()), this , SLOT(updateServerProgress3()));
    //commandTimer->start(100);
    connect(tcpServerConnection4, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress4()));
    //timer4->start();
}

void Player::updatenvdaTextServerProgress()
{

    //This code handles incoming text from nvda

    QString result(nvdaTextServerConnection->readAll());
    if (result != "")
    {
        qDebug() << result;
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

void Player::updateServerProgress3()
{

    QString result(tcpServerConnection3->readAll());
    if (result != "")
    {
        if (result.contains("Cancel"))
            stop();
        else if (result.contains("Pause"))
            pause();
        else if (result.contains("Start"))
            resume();
    }
}

void Player::updateServerProgress4()
{

    //    QString result(tcpServerConnection4->readAll());
    //    if (result != "")
    //    {
    //        if (result.contains("Cancel"))
    //        {
    //            player.stop();
    //            if (!player.playlist()->isEmpty())
    //                player.playlist()->clear();
    //            downloadManager->clearLists();
    //            downloadManager->cancelDownload();
    //        }
    //    }
}

void Player::updateIndex()
{
    QByteArray textTemp = index.toUtf8() ;
    tcpServerConnection4->write(textTemp);
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
