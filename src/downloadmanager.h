#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QTextStream>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtMultimedia/QMediaPlayer>

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager(QStringList textList, QStringList indexList);
    ~DownloadManager();
    void setVoice(QString voice);
    void addToList(QString text, QString index);
    void clearLists();
    void cancelDownload();
    void processLists();

private:
    void performMaryTTSSpeak(QString text, QString index);
    void startRequest(QUrl url);
    void finishRequest();
    QFile *file;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool httpRequestAborted;
    QString filename;
    bool downloading;
    unsigned int count;
    QStringList textList;
    QStringList indexList;
    QString index;
    QString textToSpeak;
    QString voice;

private slots:
    void httpFinished();
    void httpReadyRead();

signals:
    void finished(QString filename, QString text, QString index);
    void cancelled();
};

#endif // DOWNLOADMANAGER_H
