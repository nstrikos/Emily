#include "player.h"
#include <QDebug>


Player::Player(DownloadManager* downloadManager)
{
    this->downloadManager = downloadManager;
    connect(downloadManager, SIGNAL(finished(QString, QString, QString)), this, SLOT(playFile(QString, QString, QString)));
    voice = googleVoice;
    downloadManager->setVoice(voice);

    //Create playlist object
    createPlayListModel();
    connect(&playlist, SIGNAL(currentIndexChanged(int)), this,  SLOT(informNVDA()));
}

Player::~Player()
{
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
}

void Player::playFile(QString filename, QString text, QString index)
{
    this->index = index;
    this->spokenText << text;
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
    downloadManager->setVoice(voice);
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

        //if (nvdaIndexServerConnection != NULL)
        if (nvdaSender.nvdaIndexServerConnection != NULL)
        {
            if (!spokenIndex.isEmpty())
            {
                QString index = spokenIndex.takeFirst();
                QString text = spokenText.takeFirst();
                if (index != "")
                {
                    QByteArray textTemp = index.toUtf8() ;
                    //nvdaIndexServerConnection->write(textTemp);
                    nvdaSender.nvdaIndexServerConnection->write(textTemp);
                    qDebug() << "Sent index:" << index << ",Text:" << text;
                }
            }
        }

        //Finally process next text
        downloadManager->processLists();
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
    if (!spokenIndex.isEmpty())
        spokenIndex.clear();
    if (!spokenText.isEmpty())
        spokenText.clear();
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
