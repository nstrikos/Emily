#ifndef NVDARECEIVER_H
#define NVDARECEIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "nvdatexthandler.h"
#include "indextextlist.h"

class NvdaReceiver : public QObject
{
    Q_OBJECT

public:
    NvdaReceiver(IndexTextList* list);
    ~NvdaReceiver();

public slots:
    void nvdaTextServerAcceptConnection();
    void updatenvdaTextServerProgress();
    void nvdaCommandServerAcceptConnection();
    void updatenvdaCommandServerProgress();

private:
    QTcpServer nvdaTextServer;
    QTcpSocket *nvdaTextServerConnection;

    QTcpServer nvdaCommandServer;
    QTcpSocket *nvdaCommandServerConnection;

    NvdaTextHandler *nvdaTextHandler;
};

#endif // NVDARECEIVER_H
