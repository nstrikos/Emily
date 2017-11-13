#include "copythread.h"
#include <QTextStream>

Copy::Copy(QString drivePath, QString voice, QString rate)
{
    this->drivePath = drivePath;
    this->voice = voice;
    this->rate = rate;

    srcPath = QDir::currentPath();
    QDir installationDir = drivePath + "/Emily";
    this->dstPath = installationDir.absolutePath();
}

Copy::~Copy()
{

}

bool Copy::cpDir(const QString &srcPath, const QString &dstPath)
{
    //Copies contents of srcPath to dstPath after deleting all contents of dstPath

    rmDir(dstPath);
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName()))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            emit increase();
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
        }
    }
    return true;
}

bool Copy::rmDir(const QString &dirPath)
{
    //Deletes all contents of dirPath

    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!rmDir(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}

void Copy::installOpenMaryConf(QString drivePath)
{
    //Search drivePath and copies openmary files to every
    //userConfig directory that exists in drivePath
    QDir dir(drivePath);
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs |  QDir::NoDotAndDotDot))
    {
        if (info.isDir())
        {
            if (info.filePath().contains("userConfig"))
            {
                QString srcItemPath = QDir::currentPath() + "/resources/openmary.py" ;
                QString dstItemPath = info.filePath() + "/synthDrivers/openmary.py";
                QFile::copy(srcItemPath, dstItemPath);
            }
            else
            {
                installOpenMaryConf(info.filePath());
            }
        }
    }
}

void Copy::installUserSettings(QString voice, QString rate)
{
    QString filename = dstPath + "/userSettings";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Text) )
    {
        file.resize(0);
        QTextStream stream( &file );
        stream << "Voice: " << voice << endl;
        stream << "Rate: " << rate;
    }
    file.close();
}

int Copy::countMaxFiles()
{
//    int count = 0;

//    QDir srcDir(srcPath);
//    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
//    {
//        QString itemPath = srcPath + "/" + info.fileName();
//        if (info.isDir())
//        {
//            //We call the overloaded method with the path as parameter
//            //This is because the method recursively calls itself
//            //For the first call the path is known
//            //To continue we need to pass the directory as the string
//            //Having two methods
//            count += countFilesToBeCopied(itemPath);
//        }
//        else if (info.isFile())
//        {
//            count++;

//        }
//    }
//    return count;
   return countFilesToBeCopied(srcPath);
}

int Copy::countFilesToBeCopied(QString dirPath)
{
    int count = 0;

    QDir srcDir(dirPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
        QString itemPath = dirPath + "/" + info.fileName();
        if (info.isDir())
        {
            count += countFilesToBeCopied(itemPath);
        }
        else if (info.isFile())
        {
            count++;

        }
    }
    return count;
}

void Copy::process()
{
    //Get destination directory
    //Clear destination directory
    //Copy contents of the source directory to the destination
    //Copy openmary files to the destination
    //Emit finished


    QDir dstDir(dstPath);

    if (dstDir.exists())
        dstDir.removeRecursively();

    cpDir(srcPath, dstPath);
    installOpenMaryConf(drivePath);
    installUserSettings(voice, rate);
    emit finished();
}


