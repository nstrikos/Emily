#include "clipboardhandler.h"

ClipboardHandler::ClipboardHandler()
{
    clipBoard = QApplication::clipboard();
    connect(clipBoard, SIGNAL(dataChanged()), this, SLOT(clipBoardChanged()));
    IsEnabled = false;
    currentText = "";
}

ClipboardHandler::~ClipboardHandler()
{
    if (this->clipBoard != NULL)
    {
        clipBoard = NULL;
    }
}

void ClipboardHandler::clipBoardChanged()
{
    if (IsEnabled)
    {
        QString text = clipBoard->text();
        if (text != currentText)
        {
            currentText = text;
            emit newClipBoardText(text);
        }
        else
        {
            currentText = "";
            emit cancel("");
        }
    }
}

void ClipboardHandler::setEnabled(bool enabled)
{
    this->IsEnabled = enabled;
}
