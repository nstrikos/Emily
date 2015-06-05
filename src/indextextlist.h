#ifndef INDEXTEXTLIST_H
#define INDEXTEXTLIST_H

#include <QStringList>
#include "downloadmanager.h"

class IndexTextList : public QObject
{
    Q_OBJECT

public:
    IndexTextList(DownloadManager* downloadManager);
    void insert(QString text, QString index);

private:
    DownloadManager *downloadManager;
};

#endif // INDEXTEXTLIST_H
