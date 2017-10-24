#ifndef TEXTMEDIATOR_H
#define TEXTMEDIATOR_H

#include "nvdatextreceiver.h"
#include "nvdatexthandler.h"
#include "downloadmanager.h"
#include "player.h"
#include "nvdasender.h"

class TextMediator :
        public NvdaTextReceiverIface,
        public NvdaTextHandlerIface,
        public DownloadManagerIface,
        public PlayerIface
{
public:
    TextMediator(NvdaTextReceiver &textReceiver,
                 NvdaTextHandler &textHandler,
                 DownloadManager &downloadManager,
                 Player &player,
                 NvdaSender &nvdaSender);
    virtual ~TextMediator();

private:
    void handleRawText(QString rawText);
    void handleNormalizedText(QString text, QString index);
    void handleWav(QBuffer* buffer, QString index);
    void sendIndex(QString index);

    NvdaTextReceiver &m_textReceiver;
    NvdaTextHandler &m_textHandler;
    DownloadManager &m_downloadManager;
    Player &m_player;
    NvdaSender &m_nvdaSender;
};

#endif // TEXTMEDIATOR_H
