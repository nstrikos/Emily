/*
 * Explanation: Emily addon sends commands through 57117 port,
 * so I start QTcpServer listening at the 57117 port
 * When there is a readyRead() signal I call getCommand()
 * and send text to m_commandReceiver.
 * When there is an error I call error() and display the error
*/

#include "nvdacommandreceiver.h"

NvdaCommandReceiver::NvdaCommandReceiver()
{
    m_commandReceiver = NULL;
    connect(&nvdaCommandServer, SIGNAL(newConnection()),
            this, SLOT(nvdaCommandServerAcceptConnection()));

    nvdaCommandServer.listen(QHostAddress::LocalHost, 57117);

}

void NvdaCommandReceiver::setReceiver(NvdaCommandReceiverIface *receiver)
{
    m_commandReceiver = receiver;
}

void NvdaCommandReceiver::nvdaCommandServerAcceptConnection()
{
    nvdaCommandServerConnection = nvdaCommandServer.nextPendingConnection();
    connect(nvdaCommandServerConnection, SIGNAL(readyRead()),
            this, SLOT(getCommand()));
    connect(nvdaCommandServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error(QAbstractSocket::SocketError)));
}

void NvdaCommandReceiver::getCommand()
{
    QString command(nvdaCommandServerConnection->readAll());
    if (m_commandReceiver != NULL)
        m_commandReceiver->receiveCommand(command);
}

void NvdaCommandReceiver::error(QAbstractSocket::SocketError arg1)
{
    Q_UNUSED(arg1);
    ;//qDebug() << "Error: " << arg1;
}

NvdaCommandReceiver::~NvdaCommandReceiver()
{
    ;//qDebug() << "NvdaCommandReceiver destructor called";
}
