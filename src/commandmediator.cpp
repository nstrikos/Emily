#include "commandmediator.h"

CommandMediator::CommandMediator(NvdaCommandReceiver &commandReceiver,
                                 Player &player,
                                 MainWindow &mainWindow,
                                 DownloadManager &downloadManager,
                                 Settings &settings) :
    m_commandReceiver(commandReceiver),
    m_player(player),
    m_mainWindow(mainWindow),
    m_downloadManager(downloadManager),
    m_settings(settings)
{
    m_commandReceiver.setReceiver(this);
    m_settings.setUpdater(this);
}

void CommandMediator::receiveCommand(QString command)
{
    if (command != "")
    {
        if (command.contains("Quit"))
        {
            m_mainWindow.quitReceived();
            m_settings.writeSettings();
        }
        if (command.contains("Cancel"))
        {
            m_player.stop();
            m_downloadManager.cancelDownload();
        }
        if (command.contains("Pause"))
            m_player.pause();
        if (command.contains("Start"))
            m_player.resume();
        if (command.contains("Rate "))
            handleRateCommand(command);
        if (command.contains("Voice "))
            handleVoiceCommand(command);
    }
}

void CommandMediator::handleVoiceCommand(QString command)
{
    QString voice;

    if (command.contains("hercules"))
        voice = herculesVoiceDisplay;
    else if (command.contains("emily"))
        voice = emilyVoiceDisplay;
    else if (command.contains("spike"))
        voice = spikeVoiceDisplay;
    else if (command.contains("cori"))
        voice = coriVoiceDisplay;
    else if (command.contains("rms"))
        voice = rmsVoiceDisplay;
    else if (command.contains("prudence"))
        voice = prudenceVoiceDisplay;
    else if (command.contains("poppy"))
        voice = poppyVoiceDisplay;
    else if (command.contains("slt"))
        voice = sltVoiceDisplay;
    else if (command.contains("bdl"))
        voice = bdlVoiceDisplay;
    else if (command.contains("obadiah"))
        voice = obadiahVoiceDisplay;
    else if (command.contains("pavoque"))
        voice = pavoqueVoiceDisplay;
    else if (command.contains("pierre"))
        voice = pierreVoiceDisplay;
    else if (command.contains("lucia"))
        voice = luciaVoiceDisplay;
    else if (command.contains("ot"))
        voice = turkishVoiceDisplay;
    else if (command.contains("nk"))
        voice = teluguVoiceDisplay;

    m_downloadManager.setVoice(voice);
    m_settings.writeVoice(voice);
}

void CommandMediator::handleRateCommand(QString command)
{
    //Sometimes the command is received in this form "Voice rmsRate 50"
    //This code sends only the last part: "50"
    QString temp = command;
    int l = temp.lastIndexOf(" ");
    QString rate = temp.right(temp.length() - l);
    m_player.setRate(rate);
    m_settings.writeRate(rate);
}

void CommandMediator::updateVoice(QString voice)
{
    m_downloadManager.setVoice(voice);
}

void CommandMediator::updateRate(QString rate)
{
    m_player.setRate(rate);
}

CommandMediator::~CommandMediator()
{

}

