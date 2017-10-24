/*
 * Every class that needs to receive text from a NvdaTextHandler
 * must implement this interface
 * NvdaTextHandler receiver will call this function whenever it extracts
 * normalized text
*/

#ifndef NVDATEXTHANDLERIFACE_H
#define NVDATEXTHANDLERIFACE_H

class QString;

class NvdaTextHandlerIface
{
public:
    virtual void handleNormalizedText(QString text, QString index) = 0;
};

#endif //#ifndef NVDATEXTHANDLERIFACE_H
