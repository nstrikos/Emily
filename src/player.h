#ifndef PLAYER_H
#define PLAYER_H

#include "downloadmanager.h"
#include "nvdasender.h"
#include "constants.h"

#include <QMediaPlaylist>
#include <QtMultimedia/QMediaPlayer>
#include <QDir>
#include <QTextCodec>

class Player : public QObject
{
    Q_OBJECT

public:
    Player(DownloadManager* downloadManager);
    ~Player();
    void setVoice(QString voice);
    void increaseRate();
    void decreaseRate();
    void setRate(QString rateString);
    void stop();
    void pause();
    void resume();

public slots:    
    void speakClipBoardText(QString text);

private slots:
    void addToFileList(QString filename, QString index);
    void play();

private:
    void createPlayListModel();
    void addToPlaylist(const QString& filename);
    void clearCreatedFiles();
    void clearPlayedFiles();
    void sendIndexToNVDA();
    QMediaPlayer qMediaPlayer;
    QMediaPlaylist playlist;
    DownloadManager *downloadManager;
    QStringList playedFiles;
    QStringList createdFiles;
    QStringList createdFileIndexes;
    NvdaSender nvdaSender;
};

#endif // PLAYER_H
