#include "solution.h"
#include <QDebug>

Solution::Solution()
{
    qDebug() << "Solution creator called.";
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
    qDebug() << "Solution destructor called.";
    delete settings;
    delete commandMediator;
    delete nvdaCommandReceiver;
    delete nvdaTextReceiver;
    delete nvdaTextHandler;
    delete downloadManager;
    delete player;
    delete textMediator;
    delete nvdaSender;
    delete mainWindow;
}
