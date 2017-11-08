/*
 * Explanation: Emily addon receives indexes through 57118 port,
 * so I start QTcpServer listening at the 57118 port
 * and send an index when send() method is called.
*/

#include "nvdasender.h"
#include <QDebug>

NvdaSender::NvdaSender(QObject *parent) : QObject(parent)
{
    connect(&nvdaIndexServer, SIGNAL(newConnection()),
            this, SLOT(nvdaIndexServerAcceptConnection()));

    nvdaIndexServer.listen(QHostAddress::LocalHost, 57118);
    nvdaIndexServerConnection = NULL;
}

void NvdaSender::nvdaIndexServerAcceptConnection()
{
    nvdaIndexServerConnection = nvdaIndexServer.nextPendingConnection();
}

void NvdaSender::send(QString text)
{
    if (nvdaIndexServerConnection != NULL)
    {
        QByteArray textToSend = text.toUtf8() ;
        nvdaIndexServerConnection->write(textToSend);
    }
}

NvdaSender::~NvdaSender()
{
    nvdaIndexServer.close();
}

