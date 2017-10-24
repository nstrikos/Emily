/*
 * Class : NvdaTextReceiver
 * Role : My responsibility is to read incoming text from Nvda
 * Collaborator : When I read text from Nvda I send it to m_receiver
 * which probably is a textMediator object
*/

#ifndef NVDATEXTRECEIVER_H
#define NVDATEXTRECEIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "nvdatextreceiveriface.h"

class NvdaTextReceiver : public QObject
{
    Q_OBJECT

public:
    NvdaTextReceiver();
    ~NvdaTextReceiver();
    void setReceiver(NvdaTextReceiverIface *receiver);

private slots:
    void nvdaTextServerAcceptConnection();
    void getText();
    void error(QAbstractSocket::SocketError arg1);

private:
    QTcpServer nvdaTextServer;
    QTcpSocket *nvdaTextServerConnection;

    NvdaTextReceiverIface *m_receiver;
};

#endif //NVDATEXTRECEIVER_H
