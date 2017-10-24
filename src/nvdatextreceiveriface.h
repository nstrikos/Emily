/*
 * Every class that needs to receive text from a NvdaTextReceiver
 * must implement this interface
 * NvdaTextReceiver will call this function whenever there is an incoming text
*/

#ifndef NVDATEXTRECEIVERIFACE_H
#define NVDATEXTRECEIVERIFACE_H

class QString;

class NvdaTextReceiverIface
{
public:
    virtual void handleRawText(QString rawText) = 0;
};

#endif //NVDATEXTRECEIVERIFACE_H
