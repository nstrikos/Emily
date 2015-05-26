#include "nvdatexthandler.h"

NvdaTextHandler::NvdaTextHandler(IndexTextList* list)
{

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
                    textList << firstPart;
                    indexList << "";
                }
                incomingText = incomingText.right(incomingText.size() - indexPosition);
                int nextIndexPosition = incomingText.indexOf("#");
                QString indexString = incomingText.left(nextIndexPosition);
                indexString = indexString.replace(nvdaIndex, "");
                QString leftover = incomingText.right(incomingText.size() - nextIndexPosition - 1);
                if (!leftover.contains(nvdaIndex))
                {
                    textList << leftover;
                    indexList << indexString;
                    done = true;
                }
                else
                {
                    //Do the same again
                    int d = leftover.indexOf(nvdaIndex);
                    QString line = leftover.left(d);
                    textList << line;
                    indexList << indexString;
                    incomingText = leftover.right(leftover.size() - d);
                }
            }
        }
        else
        {
            textList << incomingText;
            indexList << "";
        }

        //Finally
        //informNVDA();
        for (int i = 0; i < textList.size(); i++)
        {
            QString text = textList.at(i);
            QString index = indexList.at(i);
            qDebug() << "index:" << index << ", text:" << text;
        }
    }
}

