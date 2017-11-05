/*
 * Class : TextMediator
 * Role : TextMediator is responsible for the communication between the objects
 * that receive text, convert it to speech and play it. With the help of TextMediator
 * the objects can stay independent from each other. It follows the mediator design pattern.
 * It implements NvdaTextReceiverIface so it can receive text from NvdaTextReceiver
 * It implements NvdaTextHandlerIface so it can receive normalized text from NvdaTextHandler
 * It implements DownloadManagerIface so it can receive wav files from DownloadManagerIface
 * It implements PlayerIface so it can receive indexes from Player
 * So we have nvdaTextReceiver to receive text from Nvda, when new text arrives,
 * handleRawText() is called and text is passed to NvdaTextHandler
 * NvdaTextHandler normalizes the text and handleNormalizedText() is called
 * which passes normalized text to DownloadManager
 * DownloadManager converts text to wav and handleWav() is called
 * which passes wav files to Player
 * Player plays the wav files and for every file that is played an index has to be passed
 * to nvda, so sendIndex() is called which passes the index to NvdaSender
 * NvdaSender sends the index to nvda
 * Collaborators : NvdaTextReceiver, NvdaTextHandler, DownloadManager, Player, NvdaSender
 */

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
