#include "nvdatexthandler.h"

NvdaTextHandler::NvdaTextHandler(IndexTextList* list)
{
    this->indexTextlist = list;
}

void NvdaTextHandler::handleText(QString incomingText)
{

    //This code handles incoming text from nvda
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
                {
                    indexTextlist->insert(firstPart, "");
                }
                incomingText = incomingText.right(incomingText.size() - indexPosition);
                int nextIndexPosition = incomingText.indexOf("#");
                QString indexString = incomingText.left(nextIndexPosition);
                indexString = indexString.replace(nvdaIndex, "");
                QString leftover = incomingText.right(incomingText.size() - nextIndexPosition - 1);
                if (!leftover.contains(nvdaIndex))
                {
                    indexTextlist->insert(leftover, indexString);
                    done = true;
                }
                else
                {
                    //Do the same again
                    int d = leftover.indexOf(nvdaIndex);
                    QString line = leftover.left(d);
                    indexTextlist->insert(line, indexString);
                    incomingText = leftover.right(leftover.size() - d);
                }
            }
        }
        else
        {
           indexTextlist->insert(incomingText, "");
        }
    }
}

