#ifndef NVDACOMMANDRECEIVERIFACE_H
#define NVDACOMMANDRECEIVERIFACE_H

class QString;

class NvdaCommandReceiverIface
{
public:
    virtual void receiveCommand(QString command) = 0;
};

#endif // NVDACOMMANDRECEIVERIFACE_H
