#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Emily"));
    ui->okButton->setFocus();

    createActions();
    createTrayAndIcons();
    createAndInitializeObjects();
    createShortcuts();
    createConnections();
    startMaryServer();
}

MainWindow::~MainWindow()
{
    if (chooseDiskDialog != NULL)
        delete chooseDiskDialog;
    if (progressDialog != NULL)
        delete progressDialog;
    delete clipboardHandler;
    delete timer;
    delete player;
    delete trayIcon;
    delete trayIconMenu;
    delete minimizeAction;
    delete quitAction;
    delete restoreAction;
    maryServerProcess.close();
    hotkeyThread.terminate();
    delete ui;
}

void MainWindow::createAndInitializeObjects()
{
    chooseDiskDialog = NULL;
    progressDialog = NULL;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(restartMaryServer()));
    timer->start(1000);
    player = new Player();

    //Create clipboard handler and connect to player slot
    clipboardHandler = new ClipboardHandler();
    connect(clipboardHandler, SIGNAL(newClipBoardText(QString)), player, SLOT(speakClipBoardText(QString)));
    connect(clipboardHandler, SIGNAL(cancel(QString)), player, SLOT(speakClipBoardText(QString)));
    IsclipBoardEnabled = false;
    hotkeyThread.start();
    connect(&hotkeyThread, SIGNAL(restoreWindow()), this, SLOT(restore()));
    connect(&hotkeyThread, SIGNAL(setGoogleGreekVoice()), this, SLOT(setGoogleGreekVoice()));
    connect(&hotkeyThread, SIGNAL(setEnglishVoice()), this, SLOT(setEnglishVoice()));
    connect(&hotkeyThread, SIGNAL(setEmilyVoice()), this, SLOT(setEmilyVoice()));
}

void MainWindow::createShortcuts()
{
    QShortcut *okShortcut = new QShortcut(QKeySequence("Esc"), this);
    connect(okShortcut, SIGNAL(activated()), this, SLOT(hide()));
    QShortcut *helpShortcut = new QShortcut(QKeySequence("F1"), this);
    connect(helpShortcut, SIGNAL(activated()), this, SLOT(help()));
    QShortcut *restartShortcut = new QShortcut(QKeySequence("F2"), this);
    connect(restartShortcut, SIGNAL(activated()), this, SLOT(startMaryServer()));
    QShortcut *installAddonShortcut = new QShortcut(QKeySequence("F3"), this);
    connect(installAddonShortcut, SIGNAL(activated()), this, SLOT(installAddon()));
    QShortcut *memoryShortcut = new QShortcut(QKeySequence("F4"), this);
    connect(memoryShortcut, SIGNAL(activated()), this, SLOT(displayMemoryStatus()));
    QShortcut *installDiskDriveShortcut = new QShortcut(QKeySequence("F5"), this);
    connect(installDiskDriveShortcut, SIGNAL(activated()), this, SLOT(installDiskDrive()));
    QShortcut *clipBoardShortcut = new QShortcut(QKeySequence("F6"), this);
    connect(clipBoardShortcut, SIGNAL(activated()), this, SLOT(enableClipBoard()));
    QShortcut *aboutShortcut = new QShortcut(QKeySequence("F7"), this);
    connect(aboutShortcut, SIGNAL(activated()), this, SLOT(about()));
    QShortcut *quitShortcut = new QShortcut(QKeySequence("F8"), this);
    connect(quitShortcut, SIGNAL(activated()), qApp, SLOT(quit()));
}

void MainWindow::createConnections()
{
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(help()));
    connect(ui->restartButton, SIGNAL(clicked()), this, SLOT(startMaryServer()));
    connect(ui->installDriversButton, SIGNAL(clicked()), this, SLOT(installAddon()));
    connect(ui->memoryButton, SIGNAL(clicked()), this, SLOT(displayMemoryStatus()));
    connect(ui->installDiskButton, SIGNAL(clicked()), this, SLOT(installDiskDrive()));
    connect(ui->enableClipboardButton, SIGNAL(clicked()), this, SLOT(enableClipBoard()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
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

void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Ε&λαχιστοποίηση"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("Ε&παναφορά"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(restore()));

    quitAction = new QAction(tr("&Έξοδος"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayAndIcons()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon icon = QIcon(":/new/prefix1/resources/flag.png");
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
//    HANDLE hProcess;
//    PROCESS_MEMORY_COUNTERS pmc;


//    if (maryServerProcess.pid() != 0)
//    {
//        hProcess = maryServerProcess.pid()->hProcess;
//        if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
//        {
//            qWarning() << (double) pmc.WorkingSetSize/MEGABYTE;
//            return (double) pmc.WorkingSetSize/MEGABYTE;
//            //CloseHandle( hProcess );
//        }
//    }
//    else
//        qWarning() << "Null process";

//    //error if control reaches here
//    return -1;

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
        msgBox.setText( QObject::tr("Η Έμιλυ δεν μπορεί να ξεκινήσει.") );
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
        //qWarning() << "Available memory found: " + QString::number(availableMemory) + " Mb";
        //qWarning() << "Mary server tries to start with " + QString::number(memory)+ "MB in Java Virtual Machine";

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
    QString file1, file2;

    QString dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    file1 = dir + "\\AppData\\Roaming\\nvda\\addons\\Emily\\synthDrivers\\openmary.py";
    file2 = dir + "\\AppData\\Roaming\\nvda\\addons\\Emily\\synthDrivers\\openmary.pyo";

    QFile::remove(file2);

    QFile::copy(":/new/prefix1/resources/openmary.py", file1);
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
    msgBox.setText("Emily 0.7\nEmail : nstrikos@yahoo.gr");
    msgBox.setIcon( QMessageBox::Information );
    msgBox.exec();
}

void MainWindow::setEnglishVoice()
{
    player->setVoice(englishVoice);
}

void MainWindow::setGoogleGreekVoice()
{
    player->setVoice(googleVoice);
}

void MainWindow::setEmilyVoice()
{
    player->setVoice(emilyVoice);
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
                Copy* copy = new Copy(drivePath);
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
        msgBox.setText("Δεν υπάρχουν δίσκοι για να γίνει εγκατάσταση");
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
    msgBox.setText("Η εγκατάσταση ολοκληρώθηκε");
    msgBox.setIcon( QMessageBox::Information );
    msgBox.exec();
}

void MainWindow::increaseRate()
{
    player->increaseRate();
}

void MainWindow::decreaseRate()
{
    player->decreaseRate();
}

void MainWindow::enableClipBoard()
{
    if (IsclipBoardEnabled)
    {
        IsclipBoardEnabled = false;
        ui->enableClipboardButton->setText(tr("F6 - Ενεργοποίηση προχείρου"));
    }
    else
    {
        ui->enableClipboardButton->setText(tr("F6 - Απενεργοποίηση προχείρου"));
        IsclipBoardEnabled = true;
    }
    clipboardHandler->setEnabled(IsclipBoardEnabled);
}

void MainWindow::stopPlayer()
{
    player->stopPlayer();
}

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
