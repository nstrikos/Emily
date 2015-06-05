#include "nvdareceiver.h"

NvdaReceiver::NvdaReceiver(IndexTextList* list, Player* player)
{
    connect(&nvdaTextServer, SIGNAL(newConnection()),
            this, SLOT(nvdaTextServerAcceptConnection()));
    connect(&nvdaCommandServer, SIGNAL(newConnection()),
            this, SLOT(nvdaCommandServerAcceptConnection()));
    nvdaTextServer.listen(QHostAddress::LocalHost, 57116);
    nvdaCommandServer.listen(QHostAddress::LocalHost, 57117);

    nvdaTextHandler = new NvdaTextHandler(list);
    nvdaCommandHandler = new NvdaCommandHandler(player);
}

NvdaReceiver::~NvdaReceiver()
{
    delete nvdaTextHandler;
    delete nvdaCommandHandler;
}

void NvdaReceiver::nvdaTextServerAcceptConnection()
{
    nvdaTextServerConnection = nvdaTextServer.nextPendingConnection();
    connect(nvdaTextServerConnection, SIGNAL(readyRead()), this, SLOT(updatenvdaTextServerProgress()));
}

void NvdaReceiver::nvdaCommandServerAcceptConnection()
{
    nvdaCommandServerConnection = nvdaCommandServer.nextPendingConnection();
    connect(nvdaCommandServerConnection, SIGNAL(readyRead()),
            this, SLOT(updatenvdaCommandServerProgress()));
}

void NvdaReceiver::updatenvdaTextServerProgress()
{
    QString incomingText(nvdaTextServerConnection->readAll());
    nvdaTextHandler->handleText(incomingText);
}

void NvdaReceiver::updatenvdaCommandServerProgress()
{

    QString result(nvdaCommandServerConnection->readAll());
    nvdaCommandHandler->handleCommand(result);
}

