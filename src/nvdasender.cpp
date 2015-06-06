#include "nvdasender.h"

NvdaSender::NvdaSender(QObject *parent) : QObject(parent)
{
    connect(&nvdaIndexServer, SIGNAL(newConnection()),
            this, SLOT(nvdaIndexServerAcceptConnection()));

    nvdaIndexServer.listen(QHostAddress::LocalHost, 57118);
    nvdaIndexServerConnection = NULL;
}

NvdaSender::~NvdaSender()
{
    nvdaIndexServer.close();
}

void NvdaSender::nvdaIndexServerAcceptConnection()
{
    nvdaIndexServerConnection = nvdaIndexServer.nextPendingConnection();
}

