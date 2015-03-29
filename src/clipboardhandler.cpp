#include "clipboardhandler.h"

ClipboardHandler::ClipboardHandler()
{
    clipBoard = QApplication::clipboard();
    connect(clipBoard, SIGNAL(dataChanged()), this, SLOT(clipBoardChanged()));
    IsEnabled = false;
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
        emit newClipBoardText(text);
    }
}

void ClipboardHandler::setEnabled(bool enabled)
{
    this->IsEnabled = enabled;
}

void ClipboardHandler::clear()
{
    clipBoard->clear();
}
