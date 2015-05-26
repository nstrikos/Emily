#ifndef NVDATEXTHANDLER_H
#define NVDATEXTHANDLER_H

#include <QString>
#include "constants.h"
#include "indextextlist.h"

#include <QDebug>

class NvdaTextHandler
{
public:
    NvdaTextHandler(IndexTextList* list);
    void handleText(QString incomingText);

private:
    QStringList textList;
    QStringList indexList;
};

#endif // NVDATEXTHANDLER_H
