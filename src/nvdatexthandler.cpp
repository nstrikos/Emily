/*
 * Explanation: handleText() gets the incoming raw text
 * and for every normalized text that is extracted,
 * sendText() is called to send normalized text to m_receiver
*/

#include "nvdatexthandler.h"
#include "constants.h"
#include <QDebug>

NvdaTextHandler::NvdaTextHandler()
{
    qDebug() << "NvdaTextHandler constructor called";
    m_receiver = NULL;
}

void NvdaTextHandler::setReceiver(NvdaTextHandlerIface *receiver)
{
    m_receiver = receiver;
}

void NvdaTextHandler::handleText(QString incomingText)
{
    //This code handles incoming text from nvda
    //Needs some cleaning here
    if (incomingText != "")
    {
        if (incomingText.contains(nvdaIndex))
        {
            bool done = false;
            while (!done)
            {
                int indexPosition = incomingText.indexOf(nvdaIndex);
                QString firstPart = incomingText.left(indexPosition);
                if (firstPart != "")
                    sendText(firstPart, "");
                incomingText = incomingText.right(incomingText.size() - indexPosition);
                int nextIndexPosition = incomingText.indexOf("#");
                QString indexString = incomingText.left(nextIndexPosition);
                indexString = indexString.replace(nvdaIndex, "");
                QString leftover = incomingText.right(incomingText.size() - nextIndexPosition - 1);
                if (!leftover.contains(nvdaIndex))
                {
                    sendText(leftover, indexString);
                    done = true;
                }
                else
                {
                    //Do the same again
                    int d = leftover.indexOf(nvdaIndex);
                    QString line = leftover.left(d);
                    sendText(line, indexString);
                    incomingText = leftover.right(leftover.size() - d);
                }
            }
        }
        else
            sendText(incomingText, "");
    }
}

void NvdaTextHandler::sendText(QString text, QString index)
{
    if (m_receiver != NULL)
        m_receiver->handleNormalizedText(text, index);
    else
        qDebug() << "NvdaTextHandlerImplementation:" <<
                    " I have text to send, but I have nowhere to send it.";
}

NvdaTextHandler::~NvdaTextHandler()
{
    qDebug() << "NvdaTextHandler destructor called";
}
