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

public slots:
    void send(QString text);

private slots:
    void nvdaIndexServerAcceptConnection();

private:
    QTcpServer nvdaIndexServer;
    QTcpSocket *nvdaIndexServerConnection;

};

#endif // NVDASENDER_H
