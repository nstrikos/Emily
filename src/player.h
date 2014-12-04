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
    Player();
    ~Player();
    void setVoice(QString voice);
    void increaseRate();
    void decreaseRate();

    void setClipboardEnabled(bool value);
    void stopPlayer();
    void clearFiles();

public slots:
    void acceptConnection();
    void updateServerProgress();
    void acceptConnection2();
    void updateServerProgress2();
    void acceptConnection3();
    void updateServerProgress3();
    void acceptConnection4();
    void updateServerProgress4();
    void updateIndex();
    void speakClipBoardText(QString text);

private slots:
    void playFile(QString filename, QString text, QString index);
    void informNVDA();

private:
    void createPlayListModel();
    void addToPlaylist(const QString& filename);
    QMediaPlayer player;
    QMediaPlaylist playlist;
    //QFileSystemWatcher fileSystemWatcher;
    //QFileSystemWatcher fileSystemWatcher4;
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
    float rate;

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    QTimer *textTimer;

    QTcpServer tcpServer2;
    QTcpSocket *tcpServerConnection2;
    QTimer *indexTimer;

    QTcpServer tcpServer3;
    QTcpSocket *tcpServerConnection3;

    QTcpServer tcpServer4;
    QTcpSocket *tcpServerConnection4;
    QTimer *timer4;
    bool clipBoardEnabled;
};

#endif // PLAYER_H
