/*
 * Class : NvdaSender
 * Role : My responsibility is to send incoming index to Nvda
 * Collaborator : None, everyone who wants to send an index to Nvda
 * just calls send() method.
*/

#ifndef NVDASENDER_H
#define NVDASENDER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NvdaSender : public QObject
{
    Q_OBJECT
public:
    explicit NvdaSender(QObject *parent = 0);
    ~NvdaSender();
    void send(QString text);

private slots:
    void nvdaIndexServerAcceptConnection();

private:
    QTcpServer nvdaIndexServer;
    QTcpSocket *nvdaIndexServerConnection;

};

#endif // NVDASENDER_H
