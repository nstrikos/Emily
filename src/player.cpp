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
    player.setPlaybackRate(rate);

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

    if (tcpServer.listen(QHostAddress::LocalHost, 57116))
        qDebug() << tcpServer.serverPort();

    if (tcpServer2.listen(QHostAddress::LocalHost, 57117))
        qDebug() << tcpServer2.serverPort();

    if (tcpServer3.listen(QHostAddress::LocalHost, 57118))
        qDebug() << tcpServer3.serverPort();

    if (tcpServer4.listen(QHostAddress::LocalHost, 57121))
        qDebug() << tcpServer4.serverPort();

    this->index = "0";
    timer4 = new QTimer();
    connect(timer4, SIGNAL(timeout()), this, SLOT(updateIndex()));
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
}

void Player::createPlayListModel()
{
    player.setPlaylist(&playlist);
    if (!player.isAvailable())
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
    //qDebug() << "Add to playlist" << text;
    addToPlaylist(filename);
    player.play();
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
    if (voice == emilyVoice)
        rate = 0.8;
    else
        rate = 1.0;
    player.setPlaybackRate(rate);
}

void Player::increaseRate()
{
    rate += 0.05;
    player.setPlaybackRate(rate);
}

void Player::decreaseRate()
{
    rate -= 0.05;
    if ( rate <= 0)
        rate = 0.05;
    player.setPlaybackRate(rate);
}
void Player::informNVDA()
{
    QString filename;

    if (playlist.currentIndex() == -1)
    {
        playlist.clear();

//        if (!fileList.isEmpty())
//        {
//            for (int i = 0; i < fileList.size(); i++)
//            {
//                QString file = fileList.at(i);
//                QFile::remove(file);
//            }
//        }
        while (!fileList.isEmpty())
        {
            QString file = fileList.takeFirst();
            QFile::remove(file);
        }

        downloadManager->processLists();
//        QString filename = QDir::tempPath() + "/emily2.txt";
//        QFile file(filename);
//        file.open(QIODevice::WriteOnly | QIODevice::Text);
//        QTextStream out(&file);
//        out.setCodec(QTextCodec::codecForName("UTF-8"));
//        out << index.toUtf8();
//        file.close();
        //QByteArray textTemp = index.toUtf8() ;
        //tcpClient.write(textTemp);
        //qDebug() << textTemp;
        //bytesToWrite = TotalBytes - (int)tcpClient.write(QByteArray(PayloadSize, '@'));
        QByteArray textTemp = index.toUtf8() ;
        tcpServerConnection4->write(textTemp);
    }
}

void Player::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(textTimer, SIGNAL(timeout()), this , SLOT(updateServerProgress()));
    textTimer->start(10);
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

    QString result(tcpServerConnection->readAll());
    if (result != "")
    {
        qDebug() << result;
        if (lastReadIndex == "")
        {
            qDebug() << "Found empty index";
        }
        downloadManager->addToList(result,  lastReadIndex);
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
    }

}

void Player::updateServerProgress3()
{

    QString result(tcpServerConnection3->readAll());
    if (result != "")
    {
        if (result.contains("Cancel"))
        {
            player.stop();
            if (!player.playlist()->isEmpty())
                player.playlist()->clear();
            downloadManager->clearLists();
            //downloadManager->cancelDownload();
        }
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
