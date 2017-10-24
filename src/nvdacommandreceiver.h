#ifndef NVDACOMMANDRECEIVER_H
#define NVDACOMMANDRECEIVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include "nvdacommandreceiveriface.h"

class NvdaCommandReceiver : public QObject
{
    Q_OBJECT
public:
    NvdaCommandReceiver();
    ~NvdaCommandReceiver();
    void setReceiver(NvdaCommandReceiverIface *receiver);
private slots:
    void nvdaCommandServerAcceptConnection();
    void getCommand();
    void error(QAbstractSocket::SocketError arg1);

private:
    QTcpServer nvdaCommandServer;
    QTcpSocket *nvdaCommandServerConnection;

    NvdaCommandReceiverIface *m_commandReceiver;
};

#endif // NVDACOMMANDRECEIVER_H
