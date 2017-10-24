/*
 * Class : NvdaTextHandler
 * Role : My responsibility is to normalize incoming text
 * Collaborator : When I extract normalized text, I send it to m_receiver
*/

#ifndef NVDATEXTHANDLER_H
#define NVDATEXTHANDLER_H

#include <QString>
#include "nvdatexthandleriface.h"

class NvdaTextHandler
{

public:
    NvdaTextHandler();
    ~NvdaTextHandler();
    void setReceiver(NvdaTextHandlerIface *receiver);
    void handleText(QString incomingText);

private:
    void sendText(QString text, QString index);
    NvdaTextHandlerIface *m_receiver;
};

#endif // NVDATEXTHANDLER_H
