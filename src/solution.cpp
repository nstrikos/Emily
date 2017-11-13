#include "solution.h"

Solution::Solution()
{
    nvdaTextReceiver = new NvdaTextReceiver();
    nvdaTextHandler = new NvdaTextHandler();
    downloadManager = new DownloadManager();
    player = new Player();
    nvdaSender = new NvdaSender();

    textMediator = new TextMediator(*nvdaTextReceiver,
                                    *nvdaTextHandler,
                                    *downloadManager,
                                    *player,
                                    *nvdaSender);

    nvdaCommandReceiver = new NvdaCommandReceiver();
    mainWindow = new MainWindow();
    settings = new Settings();

    commandMediator = new CommandMediator(*nvdaCommandReceiver,
                                          *player,
                                          *mainWindow,
                                          *downloadManager,
                                          *settings);}

Solution::~Solution()
{
    delete settings;
    delete nvdaCommandReceiver;
    delete commandMediator;
    delete nvdaTextHandler;
    delete player;
    delete downloadManager;
    delete nvdaTextReceiver;
    delete textMediator;
    delete nvdaSender;
    delete mainWindow;
}
