#ifndef PLAYER_H
#define PLAYER_H

#include "downloadmanager.h"
#include "nvdasender.h"
#include "constants.h"

#include <QMediaPlaylist>
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
    void clearFiles();

public slots:    
    void speakClipBoardText(QString text);

private slots:
    void playFile(QString filename, QString text, QString index);
    void informNVDA();

private:
    void createPlayListModel();
    void addToPlaylist(const QString& filename);
    QMediaPlayer qMediaPlayer;
    QMediaPlaylist playlist;
    DownloadManager *downloadManager;
    QStringList textList;
    QStringList indexList;
    QStringList fileList;
    QStringList playedFiles;
    QStringList createdFiles;
    QStringList spokenIndex;
    QString voice;
    QString index;
    QString lastReadIndex;
    QStringList spokenText;

    NvdaSender nvdaSender;
};

#endif // PLAYER_H
