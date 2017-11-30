#ifndef COMMANDMEDIATOR_H
#define COMMANDMEDIATOR_H

#include <QObject>
#include "nvdacommandreceiveriface.h"
#include "settingsiface.h"
#include "settingsstorageiface.h"
#include "changevoiceiface.h"
#include "nvdacommandreceiver.h"
#include "settings.h"
#include "player.h"
#include "mainwindow.h"
#include "downloadmanager.h"
#include "settings.h"

class CommandMediator :
        public NvdaCommandReceiverIface,
        public SettingsIface,
        public SettingsStorageIface,
        public ChangeVoiceIface
{

public:
    CommandMediator(NvdaCommandReceiver &commandReceiver,
                    Player &player,
                    MainWindow &mainWindow,
                    DownloadManager &downloadManager,
                    Settings &settings);
    virtual ~CommandMediator();
    void receiveCommand(QString command);
    virtual void updateVoice(QString voice);
    virtual void updateRate(QString rate);
    virtual QString getVoice();
    virtual QString getRate();
    virtual void nextVoice();

private:
    void handleVoiceCommand(QString command);
    void handleRateCommand(QString command);

    NvdaCommandReceiver &m_commandReceiver;
    Player &m_player;
    MainWindow &m_mainWindow;
    DownloadManager &m_downloadManager;
    Settings &m_settings;
};

#endif // COMMANDMEDIATOR_H
