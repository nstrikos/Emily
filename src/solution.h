#ifndef SOLUTION_H
#define SOLUTION_H

#include "nvdatextreceiver.h"
#include "nvdatexthandler.h"
#include "nvdasender.h"
#include "downloadmanager.h"
#include "player.h"
#include "textmediator.h"
#include "nvdacommandreceiver.h"
#include "mainwindow.h"
#include "settings.h"
#include "commandmediator.h"


class Solution
{
public:
    Solution();
    ~Solution();

private:
    NvdaTextReceiver *nvdaTextReceiver;
    NvdaTextHandler *nvdaTextHandler;
    NvdaSender *nvdaSender;
    DownloadManager *downloadManager;
    Player *player;
    MainWindow *mainWindow;
    Settings *settings;
    NvdaCommandReceiver *nvdaCommandReceiver;
    TextMediator *textMediator;
    CommandMediator *commandMediator;
};

#endif // SOLUTION_H
