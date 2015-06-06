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

    QTcpServer nvdaIndexServer;
    QTcpSocket *nvdaIndexServerConnection;

public slots:
    void nvdaIndexServerAcceptConnection();

};

#endif // NVDASENDER_H
