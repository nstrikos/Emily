#include "nvdareceiver.h"

NvdaReceiver::NvdaReceiver(IndexTextList* list)
{
    connect(&nvdaTextServer, SIGNAL(newConnection()),
            this, SLOT(nvdaTextServerAcceptConnection()));
    connect(&nvdaCommandServer, SIGNAL(newConnection()),
            this, SLOT(nvdaCommandServerAcceptConnection()));
    nvdaTextServer.listen(QHostAddress::LocalHost, 57151);
    nvdaCommandServer.listen(QHostAddress::LocalHost, 57152);

    nvdaTextHandler = new NvdaTextHandler(list);
}

NvdaReceiver::~NvdaReceiver()
{
    delete nvdaTextHandler;
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
    if (result != "")
    {
        //if (result.contains("Cancel"))
        //    stop();
        //else if (result.contains("Pause"))
        //    pause();
        //else if (result.contains("Start"))
        //    resume();
    }
}

