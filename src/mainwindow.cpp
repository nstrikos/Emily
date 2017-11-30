#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Emily"));
    ui->okButton->setFocus();

    m_storage = NULL;
    m_changeVoice = NULL;

    //Waiting for readSettings is too slow
    //So clipboard settings are read first
    QSettings settings("Emily", "Emily");
    //bool clip = settings.value("Clipboard").toBool();
    //setUseClipboard(clip);

    createActions();
    createTrayAndIcons();
    createAndInitializeObjects();
    createShortcuts();
    createConnections();
    startMaryServer();
    //readSettings();
    installAddon();
}

MainWindow::~MainWindow()
{
    if (chooseDiskDialog != NULL)
        delete chooseDiskDialog;
    if (progressDialog != NULL)
        delete progressDialog;
    if (selectVoiceDialog != NULL)
        delete selectVoiceDialog;
    delete timer;
    //delete downloadManager;
    //delete player;
    //delete nvdaReceiver;
    //delete indexTextList;
    delete trayIcon;
    delete trayIconMenu;
    delete minimizeAction;
    //delete quitAction;
    delete restoreAction;
    maryServerProcess.close();
    hotkeyThread.terminate();
    //writeSettings();
    delete okShortcut;
    delete helpShortcut;
    delete installAddonShortcut;
    delete installDiskDriveShortcut;
    delete nextVoiceShortcut;
    delete aboutShortcut;
    delete ui;
}

void MainWindow::setStorage(SettingsStorageIface *storage)
{
    m_storage = storage;
}

void MainWindow::setChangeVoice(ChangeVoiceIface *changeVoice)
{
    m_changeVoice = changeVoice;
}

void MainWindow::createAndInitializeObjects()
{
    chooseDiskDialog = NULL;
    progressDialog = NULL;
    selectVoiceDialog = NULL;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(restartMaryServer()));
    timer->start(1000);
    hotkeyThread.start();
    connect(&hotkeyThread, SIGNAL(restoreWindow()), this, SLOT(restore()));
    //connect(&hotkeyThread, SIGNAL(setGreekVoice()), this, SLOT(setGreekVoice()));
    //connect(&hotkeyThread, SIGNAL(setEnglishVoice()), this, SLOT(setEnglishVoice()));
    //connect(&hotkeyThread, SIGNAL(stop()), this, SLOT(stopPlayer()));

    //This is the core algorithm
    //These objects cooperate to implement the functionality

    //downloadManager = new DownloadManager();
    //indexTextList = new IndexTextList(downloadManager);
    //player = new Player(downloadManager);
    //nvdaReceiver = new NvdaReceiver(indexTextList, player);
    //connect(&hotkeyThread, SIGNAL(speakHighlightedText(QString)), player, SLOT(speakClipBoardText(QString)));
    //connect(&hotkeyThread, SIGNAL(pause()), player, SLOT(pause()));

    //clipboard = QApplication::clipboard();
    //connect(clipboard, SIGNAL(dataChanged()), this, SLOT(speakClipboard()));
}

void MainWindow::createShortcuts()
{
    okShortcut = new QShortcut(QKeySequence("Esc"), this);
    connect(okShortcut, SIGNAL(activated()), this, SLOT(hide()));
    helpShortcut = new QShortcut(QKeySequence("F1"), this);
    connect(helpShortcut, SIGNAL(activated()), this, SLOT(help()));
    installDiskDriveShortcut = new QShortcut(QKeySequence("F2"), this);
    connect(installDiskDriveShortcut, SIGNAL(activated()), this, SLOT(installDiskDrive()));
    installAddonShortcut = new QShortcut(QKeySequence("F3"), this);
    connect(installAddonShortcut, SIGNAL(activated()), this, SLOT(installAddon()));
    nextVoiceShortcut = new QShortcut(QKeySequence("F4"), this);
    connect(nextVoiceShortcut, SIGNAL(activated()), this, SLOT(nextVoice()));
    aboutShortcut = new QShortcut(QKeySequence("F12"), this);
    connect(aboutShortcut, SIGNAL(activated()), this, SLOT(about()));
    //QShortcut *selectVoiceShortcut = new QShortcut(QKeySequence("F2"), this);
    //connect(selectVoiceShortcut, SIGNAL(activated()), this, SLOT(selectVoice()));
    //QShortcut *restartShortcut = new QShortcut(QKeySequence("F3"), this);
    //connect(restartShortcut, SIGNAL(activated()), this, SLOT(startMaryServer()));

    //QShortcut *memoryShortcut = new QShortcut(QKeySequence("F5"), this);
    //connect(memoryShortcut, SIGNAL(activated()), this, SLOT(displayMemoryStatus()));

    //QShortcut *clipboardhortcut = new QShortcut(QKeySequence("F7"), this);
    //connect(clipboardhortcut, SIGNAL(activated()), this, SLOT(clipboardButtonClicked()));
    //QShortcut *quitShortcut = new QShortcut(QKeySequence("F9"), this);
    //connect(quitShortcut, SIGNAL(activated()), qApp, SLOT(quit()));
}

void MainWindow::createConnections()
{
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(help()));
    //connect(ui->selectVoiceButton, SIGNAL(clicked(bool)), this, SLOT(selectVoice()));
    //connect(ui->restartButton, SIGNAL(clicked()), this, SLOT(startMaryServer()));
    connect(ui->installDriversButton, SIGNAL(clicked()), this, SLOT(installAddon()));
    //connect(ui->memoryButton, SIGNAL(clicked()), this, SLOT(displayMemoryStatus()));
    connect(ui->installDiskButton, SIGNAL(clicked()), this, SLOT(installDiskDrive()));
    //connect(ui->clipboardButton, SIGNAL(clicked(bool)), this, SLOT(clipboardButtonClicked()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    connect(ui->nextButton, SIGNAL(clicked(bool)), this, SLOT(nextVoice()));
    //connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
}

void MainWindow::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void MainWindow::quitReceived()
{
    qApp->quit();
}

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(restore()));

    //quitAction = new QAction(tr("E&xit"), this);
    //connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayAndIcons()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    //trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon icon = QIcon(":/new/prefix1/resources/sound.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    trayIcon->setToolTip(tr("Emily"));
    trayIcon->show();
}


void MainWindow::restore()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
    this->setFocus();
    ui->okButton->setFocus();
}

double MainWindow::getAvailableMemory()
{
    MEMORYSTATUSEX memory_status;
    ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memory_status);
    return (double) memory_status.ullAvailPhys/MEGABYTE;
}

double MainWindow::getUsedMemory()
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    if (maryServerProcess.pid() != 0)
    {
        hProcess = maryServerProcess.pid()->hProcess;
        if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
        {
            qWarning() << (double) pmc.WorkingSetSize/MEGABYTE;
            return (double) pmc.WorkingSetSize/MEGABYTE;
            //CloseHandle( hProcess );
        }
    }
    else
        qWarning() << "Null process";

    //error if control reaches here
    return -1;
}


void MainWindow::startMaryServer()
{
    double availableMemory;
    memoryForMaryServer = 0;

    maryServerProcess.close();

    availableMemory = getAvailableMemory();

    //if there is enough memory available we allocate it to mary server
    //otherwise we choose the default allocation
    if (availableMemory > MAX_MEMORY_FOR_MARY)
        //try to allocate 2Gb for mary server
        startMaryServerProcess(MAX_MEMORY_FOR_MARY);

    //if process has not started try with 1gb
    if  (maryServerProcess.pid() == 0)
    {
        if (availableMemory > NORMAL_MEMORY_FOR_MARY)
            //try to allocate 1Gb for mary server
            startMaryServerProcess(NORMAL_MEMORY_FOR_MARY);

    }

    if  (maryServerProcess.pid() == 0)
        //try to allocate 500mb for mary server
        startMaryServerProcess(MIN_MEMORY_FOR_MARY);

    //Not necessary anymore
    //Openmary.py now waits enough to connect with server
    //startNVDA();

    //finally if process fails to start we display information
    if (maryServerProcess.pid() == 0)
    {
        memoryForMaryServer = 0;
        QMessageBox msgBox;
        msgBox.setText( QObject::tr("Emily cannot start.") );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
    }
}

void MainWindow::restartMaryServer()
{
    if (maryServerProcess.pid() == NULL)
        startMaryServer();
}

void MainWindow::startMaryServerProcess(int memory)
{

    if  (maryServerProcess.pid() == 0)
    {
        //try to allocate memory for mary server
        QString directory = QDir::currentPath();
        directory.replace("/", "\\");
        QString string1 = "\"" + directory + "\\marytts\\bin\\jre\\bin\\java\" -ea -Xms40m -Xmx" + QString::number(memory) +"m -cp ";
        QString string2 = "\"" + directory + "\\marytts\\lib\\*\" \"-Dserver=http\" marytts.server.Mary\"";
        QString string3 = string1 + " " + string2;

        maryServerProcess.start(string3);
        memoryForMaryServer = memory;

        //If mary server has started this delay does not affect anything
        //If mary server has not started this delay is necessary to catch the flaw in the next check
        delay(2);
    }
}

void MainWindow::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::installAddon()
{
    QString manifestFile, openmaryFile, openmaryObjectFile;

    QString nvdaRoamingPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
            "\\AppData\\Roaming\\nvda";

    QDir nvdaDir(nvdaRoamingPath);
    if (nvdaDir.exists())
    {

        QString emilyAddonPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                "\\AppData\\Roaming\\nvda\\addons\\Emily";

        QString emilyAddonSynthDriverPath = emilyAddonPath + "\\synthDrivers";

        QDir dir(emilyAddonSynthDriverPath);
        if(!dir.exists())
            dir.mkpath(emilyAddonSynthDriverPath);

        manifestFile = emilyAddonPath + "\\manifest.ini";
        openmaryFile = emilyAddonSynthDriverPath + "\\openmary.py";
        openmaryObjectFile = emilyAddonSynthDriverPath + "\\openmary.pyo";


        QString text;
        bool needsDelete = false;
        //QString filename = QDir::currentPath() + "/userSettings";
        QFile file( manifestFile );
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);

            text = in.readAll();
            if (!text.contains("version = 0.9"))
                needsDelete = true;
        }

        file.close();

//        qDebug() << file.size();
//        qDebug() << text;
//        qDebug() << needsDelete;

        if (needsDelete == true)
        {
            QFile::remove(manifestFile);
            QFile::remove(openmaryFile);
            QFile::remove(openmaryObjectFile);
        }

        if (!QFile::exists(openmaryFile) || (needsDelete == true) )
        {
            //QFile::remove(openmaryObjectFile);

            QFile::copy(":/new/prefix1/resources/manifest.ini", manifestFile);
            QFile::copy(":/new/prefix1/resources/openmary.py", openmaryFile);

            QMessageBox msgBox;
            msgBox.setText(tr("Addon installation is complete"));
            msgBox.setIcon( QMessageBox::Information );
            msgBox.exec();
        }
    }
}

void MainWindow::displayMemoryStatus()
{
    QMessageBox msgBox;
    msgBox.setText("Διαθέσιμη μνήμη : " + QString::number(round(getAvailableMemory())) + " Megabyte\n" +
                   "Μνήμη σε χρήση : " + QString::number(round(getUsedMemory())) + " Megabyte \n" +
                   "Μέγιστη μνήμη : " + QString::number(memoryForMaryServer) + " Megabyte");
    msgBox.setIcon( QMessageBox::Information );
    msgBox.exec();
}

void MainWindow::help()
{
    QString directory = QDir::currentPath();
    QString helpFile = directory += "/resources/help.txt";
    QString resource = "file:///" + helpFile;
    QDesktopServices::openUrl(QUrl(resource, QUrl::TolerantMode));
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setText("Emily 0.9\nEmail : nstrikos@yahoo.gr\nIcons: Linecons Free by Designmodo\nLicense: Creative Commons");
    msgBox.setIcon( QMessageBox::Information );
    msgBox.exec();
}

void MainWindow::installDiskDrive()
{
    if (!chooseDiskDialog)
        chooseDiskDialog = new ChooseDiskDialog(this);

    if (chooseDiskDialog->searchDrivesAndAddtoCombobox())
    {
        if (chooseDiskDialog->exec())
        {
            QString drivePath = chooseDiskDialog->getDrivePath();
            if (drivePath != "")
            {
                QDir installationDir = drivePath + "/Emily";
                if (!installationDir.exists())
                    installationDir.mkdir(".");

                //Create dialog before connecting signals from the thread
                if (!progressDialog)
                    progressDialog = new ProgressDialog(this);

                //Set up thread
                QThread* copyThread = new QThread;
                Copy* copy = new Copy(drivePath, m_storage->getVoice(), m_storage->getRate());
                int maxFiles = copy->countMaxFiles();
                copy->moveToThread(copyThread);
                connect(copyThread, SIGNAL(started()), copy, SLOT(process()));
                connect(copy, SIGNAL(finished()), copyThread, SLOT(quit()));

                //We schedule the objects for deletion, no leak memory here
                connect(copy, SIGNAL(finished()), copy, SLOT(deleteLater()));
                connect(copyThread, SIGNAL(finished()), copyThread, SLOT(deleteLater()));
                connect(copy, SIGNAL(increase()), progressDialog, SLOT(increase()));
                connect(copy, SIGNAL(finished()), this, SLOT(installationComplete()));

                //Activate progress dialog
                progressDialog->resetProgressBar(maxFiles);
                progressDialog->show();
                progressDialog->raise();
                copyThread->start();
            }
        }
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("There are no disk drives to install"));
        msgBox.setIcon( QMessageBox::Information );
        msgBox.exec();
    }
}

void MainWindow::installationComplete()
{
    progressDialog->setCanClose(true);
    progressDialog->close();
    if (progressDialog != NULL)
    {
        delete progressDialog;
        progressDialog = NULL;
    }

    QMessageBox msgBox;
    msgBox.setText(tr("Installation completed"));
    msgBox.setIcon( QMessageBox::Information );
    msgBox.exec();
}

void MainWindow::nextVoice()
{
    if (m_changeVoice != NULL)
        m_changeVoice->nextVoice();
}

//void MainWindow::increaseRate()
//{
//    player->increaseRate();
//}

//void MainWindow::decreaseRate()
//{
//    player->decreaseRate();
//}

//void MainWindow::stopPlayer()
//{
//    player->stop();
//}

//void MainWindow::startNVDA()
//{
//    This is not necessary because NVDA addon waits for the server to come up
//    INPUT ip;
//    ip.type = INPUT_KEYBOARD;
//    ip.ki.wScan = 0;
//    ip.ki.time = 0;
//    ip.ki.dwExtraInfo = 0;


//    // Press the "Ctrl" key
//    ip.ki.wVk = VK_CONTROL;
//    ip.ki.dwFlags = 0; // 0 for key press
//    SendInput(1, &ip, sizeof(INPUT));

//    // Press the "ALT" key
//    ip.ki.wVk = VK_MENU;
//    ip.ki.dwFlags = 0; // 0 for key press
//    SendInput(1, &ip, sizeof(INPUT));

//    // Press the "N" key
//    ip.ki.wVk = 'N';
//    ip.ki.dwFlags = 0; // 0 for key press
//    SendInput(1, &ip, sizeof(INPUT));

//    // Release the "N" key
//    ip.ki.wVk = 'N';
//    ip.ki.dwFlags = KEYEVENTF_KEYUP;
//    SendInput(1, &ip, sizeof(INPUT));

//    // Release the "ALT" key
//    ip.ki.wVk = VK_MENU;
//    ip.ki.dwFlags = KEYEVENTF_KEYUP;
//    SendInput(1, &ip, sizeof(INPUT));

//    // Release the "Ctrl" key
//    ip.ki.wVk = VK_CONTROL;
//    ip.ki.dwFlags = KEYEVENTF_KEYUP;
//    SendInput(1, &ip, sizeof(INPUT));
//}

//void MainWindow::writeSettings()
//{
//    QSettings settings("Emily", "Emily");
//    settings.setValue("Voice", this->voice);
//    //settings.setValue("Clipboard", useClipboard);
//}

//void MainWindow::readSettings()
//{
//    QSettings settings("Emily", "Emily");
//    QString readVoice = settings.value("Voice").toString();
//    setVoice(readVoice);

////    This code helps to find the path of the settings file
////    QString config_dir = QFileInfo(settings.fileName()).absolutePath() + "/";
////    qDebug() << config_dir;
//}

//void MainWindow::setVoice(QString voice)
//{
//    if (voice != "")
//        this->voice = voice;
//    else
//        this->voice = googleVoice;
//    player->setVoice(this->voice);
//}

//void MainWindow::selectVoice()
//{
//    if (selectVoiceDialog == NULL)
//        selectVoiceDialog = new SelectVoiceDialog();
//    selectVoiceDialog->setModal(true);
//    selectVoiceDialog->initCombobox(this->voice);

//    if (selectVoiceDialog->exec())
//    {
//        this->voice = selectVoiceDialog->getSelectedVoice();
//        //player->setVoice(this->voice);
//    }
//}

//void MainWindow::speakClipboard()
//{
//    if (useClipboard)
//        player->speakClipBoardText(clipboard->text());
//}

//void MainWindow::setUseClipboard(bool value)
//{
//    this->useClipboard = value;
//    if (useClipboard)
//    {
//        ui->clipboardButton->setText(tr("F7 - Χρήση προχείρου ενεργή"));
//    }
//    else
//        ui->clipboardButton->setText(tr("F7 - Χρήση προχείρου μη ενεργή"));
//}

//void MainWindow::clipboardButtonClicked()
//{
//    setUseClipboard(!useClipboard);
//}
