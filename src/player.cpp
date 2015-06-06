#include "player.h"

Player::Player(DownloadManager* downloadManager)
{
    this->downloadManager = downloadManager;
    connect(downloadManager, SIGNAL(finished(QString, QString)), this, SLOT(addToFileList(QString, QString)));

    //Create playlist object
    createPlayListModel();
    connect(&playlist, SIGNAL(currentIndexChanged(int)), this,  SLOT(play()));
}

Player::~Player()
{
    //Clear all files
    clearPlayedFiles();
    clearCreatedFiles();
}

void Player::createPlayListModel()
{
    qMediaPlayer.setPlaylist(&playlist);
}

void Player::addToFileList(QString filename, QString index)
{
    createdFiles << filename;
    createdFileIndexes << index;
    play();
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

void Player::play()
{
    //This function is called when player has finished playing
    //If playlist is not empty we do nothing, the next file will be played automatically
    //Otherwise we do the next
    if (playlist.currentIndex() == -1)
    {
        //Clear playlist, clear all the files that have been played so far
        playlist.clear();
        clearPlayedFiles();

        //Take one file from the createdFiles list and put it in playedFiles list
        //add it to play list, start the player and send the index to nvda
        if (!createdFiles.isEmpty())
        {
            QString filename = createdFiles.takeFirst();
            playedFiles << filename;
            addToPlaylist(filename);
            qMediaPlayer.play();
        }

        sendIndexToNVDA();

        //Finally process next text
        downloadManager->processLists();
    }
}

void Player::speakClipBoardText(QString text)
{
    clearCreatedFiles();
    clearPlayedFiles(); // ????? is it necessary?
    qMediaPlayer.stop();
    if (!qMediaPlayer.playlist()->isEmpty())
        qMediaPlayer.playlist()->clear();
    downloadManager->clearLists();
    createdFiles.clear();
    downloadManager->addToClipboardList(text);
    play();
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
    //createdFiles.clear();
    if (!createdFileIndexes.isEmpty())
        createdFileIndexes.clear();
    clearCreatedFiles();
    clearPlayedFiles();
}

void Player::pause()
{
    qMediaPlayer.pause();
}

void Player::resume()
{
    qMediaPlayer.play();
}

void Player::clearCreatedFiles()
{
    while (!createdFiles.isEmpty())
    {
        QString file = createdFiles.takeFirst();
        QFile::remove(file);
    }
}

void Player::clearPlayedFiles()
{
    while (!playedFiles.isEmpty())
    {
        QString file = playedFiles.takeFirst();
        QFile::remove(file);
    }
}

void Player::sendIndexToNVDA()
{
    if (nvdaSender.nvdaIndexServerConnection != NULL)
    {
        if (!createdFileIndexes.isEmpty())
        {
            QString indexToSend = createdFileIndexes.takeFirst();
            if (indexToSend != "")
            {
                QByteArray textToSend = indexToSend.toUtf8() ;
                nvdaSender.nvdaIndexServerConnection->write(textToSend);
            }
        }
    }
}
