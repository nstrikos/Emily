#include "player.h"

static const int TotalBytes = 50 * 1024 * 1024;
static const int PayloadSize = 64 * 1024; // 64 KB

Player::Player()
{
    downloadManager = new DownloadManager(textList, indexList);
    createPlayListModel();
    connect(downloadManager, SIGNAL(finished(QString, QString, QString)), this, SLOT(playFile(QString, QString, QString)));
    connect(&playlist, SIGNAL(currentIndexChanged(int)), this,  SLOT(informNVDA()));
    voice = googleVoice;
    downloadManager->setVoice(voice);
    rate = 1.0;
    qMediaPlayer.setPlaybackRate(rate);

    textTimer = new QTimer();
    indexTimer = new QTimer();

    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));

    connect(&tcpServer2, SIGNAL(newConnection()),
            this, SLOT(acceptConnection2()));

    connect(&tcpServer3, SIGNAL(newConnection()),
            this, SLOT(acceptConnection3()));

    connect(&tcpServer4, SIGNAL(newConnection()),
            this, SLOT(acceptConnection4()));

    tcpServer.listen(QHostAddress::LocalHost, 57116);
    tcpServer2.listen(QHostAddress::LocalHost, 57117);
    tcpServer3.listen(QHostAddress::LocalHost, 57118);
    tcpServer4.listen(QHostAddress::LocalHost, 57121);


    this->index = "0";
    timer4 = new QTimer();
    connect(timer4, SIGNAL(timeout()), this, SLOT(updateIndex()));
    tcpServerConnection4 = NULL;
    clipBoardEnabled = false;
}

Player::~Player()
{
    tcpServer.close();
    tcpServer2.close();
    tcpServer3.close();
    tcpServer4.close();
    if (downloadManager != NULL)
    {
        delete downloadManager;
        downloadManager = NULL;
    }
    while (!playedFiles.isEmpty())
    {
        QString file = playedFiles.takeFirst();
        QFile::remove(file);
    }
    while (!createdFiles.isEmpty())
    {
        QString file = createdFiles.takeFirst();
        QFile::remove(file);
    }
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
            qDebug() << file;
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

void Player::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    //connect(textTimer, SIGNAL(timeout()), this , SLOT(updateServerProgress()));
    //textTimer->start(10);
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));
}

void Player::acceptConnection2()
{
    tcpServerConnection2 = tcpServer2.nextPendingConnection();
    connect(indexTimer, SIGNAL(timeout()), this , SLOT(updateServerProgress2()));
    indexTimer->start(10);
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

void Player::updateServerProgress()
{

    //This code can handle both formats of openmary.py
    //Sending self.index at the end of the text
    //Or in the middle

    const QString nvdaIndex = "(NVDA Index)";
    QString result(tcpServerConnection->readAll());
    if (result != "")
    {
        //qDebug() << "Incoming text:" << result;
        if (result.contains(nvdaIndex))
        {
            bool done = false;
            while (!done)
            {
                int c = result.indexOf(nvdaIndex);
                QString preLine = result.left(c);
                if (preLine != "")
                {
                    //qDebug() << "Preline:" << preLine;
                    downloadManager->addToList(preLine, "");
                }
                result = result.right(result.size() - c);
                //qDebug() << "Result:" << result;
                int n = result.indexOf("#");
                QString index = result.left(n);
                index = index.replace(nvdaIndex, "");
                //qDebug() << "Index:" << index;
                QString leftover = result.right(result.size() - n - 1);
                //qDebug() << "Leftover:" << leftover;
                if (!leftover.contains(nvdaIndex))
                {
                    QString line = leftover;
                    downloadManager->addToList(line, index);
                    done = true;
                }
                else
                {
                    //qDebug() << "Leftover:" << leftover;
                    int d = leftover.indexOf(nvdaIndex);
                    QString line = leftover.left(d);
                    downloadManager->addToList(line, index);
                    //qDebug() << "Line:" << line;
                    result = leftover.right(leftover.size() - d);
                    //qDebug() << "New result:" << result;
                }
                //qDebug() << "Line:" << line;
                //downloadManager->addToList(line, index);
            }

        }
        else
        {
            QString line = result;
            QString index = "";
            //qDebug() << "Index:" << index;
            //qDebug() << "Line:" << line;
            downloadManager->addToList(result,  "");
        }
        //downloadManager->addToList(result,  lastReadIndex);
        informNVDA();
    }
}

void Player::updateServerProgress2()
{

    QString result(tcpServerConnection2->readAll());
    if (result != "")
    {
        int n = result.lastIndexOf("Index:");
        QString index = result.right(result.size() - n);
        index.replace("Index:", "");
        lastReadIndex = index;
        //qDebug() << "Read index:" << lastReadIndex;
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

void Player::setClipboardEnabled(bool value)
{
    this->clipBoardEnabled = value;
    downloadManager->setClipBoardEnabled(value);
}

void Player::stop()
{
    clearFiles();
    downloadManager->clearLists();
    downloadManager->cancelDownload();
    fileList.clear();
    indexList.clear();
    if (!qMediaPlayer.playlist()->isEmpty())
        qMediaPlayer.playlist()->clear();

    //We stop player after cleaning all
    //Because stopping player will insert new media in playlist
    //We do this because this affects NVDA
    qMediaPlayer.stop();
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
