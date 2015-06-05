#ifndef PLAYER_H
#define PLAYER_H

#include "downloadmanager.h"
#include "constants.h"

#include <QMediaPlaylist>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class QTcpServer;
class QTcpSocket;

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

    void nvdaIndexServerAcceptConnection();
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
    QString spokenText;

    QTcpServer nvdaIndexServer;
    QTcpSocket *nvdaIndexServerConnection;
};

#endif // PLAYER_H
