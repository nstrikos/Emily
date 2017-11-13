#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <QDir>

class Copy : public QObject {
    Q_OBJECT

public:
    Copy(QString drivePath, QString voice, QString rate);
    ~Copy();
    int countMaxFiles();

public slots:
    void process();

signals:
    void finished();
    void increase();

private:
    QString srcPath;
    QString dstPath;
    QString drivePath;
    QString voice;
    QString rate;
    bool cpDir(const QString &srcPath, const QString &dstPath);
    bool rmDir(const QString &dirPath);
    void installOpenMaryConf(QString searchPath);
    void installUserSettings(QString voice, QString rate);
    int countFilesToBeCopied(QString dirPath);
};



#endif // COPYTHREAD_H
