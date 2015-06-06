#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtNetwork/QNetworkReply>
#include <QDir>

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager();
    ~DownloadManager();
    void setVoice(QString voice);
    void addToList(QString text, QString index);
    void addToClipboardList(QString text);
    void setClipBoardEnabled(bool value);
    void clearLists();
    void cancelDownload();

public slots:
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
    bool clipBoardEnabled;

private slots:
    void httpFinished();
    void httpReadyRead();

signals:
    void finished(QString filename, QString index);
    void cancelled();
};

#endif // DOWNLOADMANAGER_H
