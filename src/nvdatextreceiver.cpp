/*
 * Explanation: Emily addon sends text through 57116 port,
 * so I start QTcpServer listening at the 57116 port
 * When there is a readyRead() signal I call getText() and send text to m_receiver.
 * When there is an error I call error() and display the error
*/

#include "nvdatextreceiver.h"

NvdaTextReceiver::NvdaTextReceiver()
{
    m_receiver = NULL;
    connect(&nvdaTextServer, SIGNAL(newConnection()),
            this, SLOT(nvdaTextServerAcceptConnection()));

    nvdaTextServer.listen(QHostAddress::LocalHost, 57116);
}

void NvdaTextReceiver::setReceiver(NvdaTextReceiverIface *receiver)
{
    m_receiver = receiver;
}

void NvdaTextReceiver::nvdaTextServerAcceptConnection()
{
    nvdaTextServerConnection = nvdaTextServer.nextPendingConnection();
    connect(nvdaTextServerConnection, SIGNAL(readyRead()), this, SLOT(getText()));
    connect(nvdaTextServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error(QAbstractSocket::SocketError)));
}

void NvdaTextReceiver::getText()
{
    QString text(nvdaTextServerConnection->readAll());
    if (m_receiver != NULL)
        m_receiver->handleRawText(text);
}

void NvdaTextReceiver::error(QAbstractSocket::SocketError arg1)
{
    Q_UNUSED(arg1);
    ;//qDebug() << "Error: " << arg1;
}

NvdaTextReceiver::~NvdaTextReceiver()
{
    ;//qDebug() << "NvdaTextGetter destructor called";
}
