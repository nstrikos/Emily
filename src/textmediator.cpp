#include "textmediator.h"

TextMediator::TextMediator(NvdaTextReceiver &textReceiver,
                           NvdaTextHandler &textHandler,
                           DownloadManager &downloadManager,
                           Player &player,
                           NvdaSender &nvdaSender) :
    m_textReceiver(textReceiver),
    m_textHandler(textHandler),
    m_downloadManager(downloadManager),
    m_player(player),
    m_nvdaSender(nvdaSender)
{
    m_textReceiver.setReceiver(this);
    m_textHandler.setReceiver(this);
    m_downloadManager.setWavReceiver(this);
    m_player.setIndexHandler(this);
}

void TextMediator::handleRawText(QString rawText)
{
    m_textHandler.handleText(rawText);
}

void TextMediator::handleNormalizedText(QString text, QString index)
{
    m_downloadManager.textToSpeech(text, index);
}

void TextMediator::handleWav(QBuffer *buffer, QString index)
{
    m_player.playFile(buffer, index);
}

void TextMediator::sendIndex(QString index)
{
    m_nvdaSender.send(index);
}

TextMediator::~TextMediator()
{

}
