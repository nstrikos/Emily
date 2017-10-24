#include "nvdacommandreceiver.h"
#include <QDebug>

NvdaCommandReceiver::NvdaCommandReceiver()
{
    qDebug() << "NvdaCommandReceiver constructor called";
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
    else
        qDebug() << "NvdaCommandReceiver: I have a command to send, but I have nowhere to send it";
}

void NvdaCommandReceiver::error(QAbstractSocket::SocketError arg1)
{
    qDebug() << "Error: " << arg1;
}

NvdaCommandReceiver::~NvdaCommandReceiver()
{
    qDebug() << "NvdaCommandReceiver destructor called";
}
