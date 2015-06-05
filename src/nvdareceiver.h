#ifndef NVDARECEIVER_H
#define NVDARECEIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "nvdatexthandler.h"
#include "indextextlist.h"
#include "nvdacommandhandler.h"
#include "player.h"

class NvdaReceiver : public QObject
{
    Q_OBJECT

public:
    NvdaReceiver(IndexTextList* list, Player* player);
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
    NvdaCommandHandler *nvdaCommandHandler;
};

#endif // NVDARECEIVER_H
