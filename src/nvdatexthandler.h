#ifndef NVDATEXTHANDLER_H
#define NVDATEXTHANDLER_H

#include <QString>
#include "constants.h"
#include "indextextlist.h"

class NvdaTextHandler
{
public:
    NvdaTextHandler(IndexTextList* list);
    void handleText(QString incomingText);

private:
    IndexTextList* indexTextlist;
};

#endif // NVDATEXTHANDLER_H
