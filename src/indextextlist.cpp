#include "indextextlist.h"

IndexTextList::IndexTextList(DownloadManager* downloadManager)
{
    this->downloadManager = downloadManager;
}

void IndexTextList::insert(QString text, QString index)
{
    downloadManager->addToList(text, index);
}
