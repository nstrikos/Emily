#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <windows.h>
#include <psapi.h>

#define MEGABYTE 1048576
#define MAX_MEMORY_FOR_MARY 2000 // 2000 megabytes
#define NORMAL_MEMORY_FOR_MARY 1000 //1000 megabytes
#define MIN_MEMORY_FOR_MARY 500 //500 megabytes

//Ok
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();

    QIcon icon = QIcon(":/new/prefix1/resources/flag.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    trayIcon->setToolTip(tr("Emily"));
    trayIcon->show();

    setWindowTitle(tr("Emily"));
    //resize(800, 500);
    chooseDiskDialog = NULL;
    progressDialog = NULL;

    QxtGlobalShortcut *globalRestoreShortcut = new QxtGlobalShortcut(this);

    globalRestoreShortcut->setShortcut(QKeySequence("Ctrl+Alt+e"));
    connect(globalRestoreShortcut, SIGNAL(activated()), this, SLOT(restore())) ;

    QxtGlobalShortcut *globalRestartShortcut = new QxtGlobalShortcut(this);

    globalRestartShortcut->setShortcut(QKeySequence("Ctrl+Alt+r"));
    connect(globalRestartShortcut, SIGNAL(activated()), this, SLOT(startMaryServer())) ;

    QxtGlobalShortcut *globalGoogleGreekVoice = new QxtGlobalShortcut(this);

    globalGoogleGreekVoice->setShortcut(QKeySequence("Ctrl+Alt+1"));
    connect(globalGoogleGreekVoice, SIGNAL(activated()), this, SLOT(setGoogleGreekVoice())) ;


    QxtGlobalShortcut *globalEnglishVoice = new QxtGlobalShortcut(this);

    globalEnglishVoice->setShortcut(QKeySequence("Ctrl+Alt+2"));
    connect(globalEnglishVoice, SIGNAL(activated()), this, SLOT(setEnglishVoice())) ;

    QxtGlobalShortcut *globalEmilyVoice = new QxtGlobalShortcut(this);

    globalEmilyVoice->setShortcut(QKeySequence("Ctrl+Alt+3"));
    connect(globalEmilyVoice, SIGNAL(activated()), this, SLOT(setEmilyVoice())) ;

    QxtGlobalShortcut *globalIncreaseRate = new QxtGlobalShortcut(this);

    globalIncreaseRate->setShortcut(QKeySequence("Ctrl+Alt++"));
    connect(globalIncreaseRate, SIGNAL(activated()), this, SLOT(increaseRate())) ;

    QxtGlobalShortcut *globalDecreaseRate = new QxtGlobalShortcut(this);

    globalDecreaseRate->setShortcut(QKeySequence("Ctrl+Alt+-"));
    connect(globalDecreaseRate, SIGNAL(activated()), this, SLOT(decreaseRate())) ;


    QxtGlobalShortcut *globalPlayerStop = new QxtGlobalShortcut(this);
    globalPlayerStop->setShortcut(QKeySequence("Ctrl+Alt+s"));
    connect(globalPlayerStop, SIGNAL(activated()), this, SLOT(stopPlayer())) ;

    QShortcut *okShortcut = new QShortcut(QKeySequence("Esc"), this);
    connect(okShortcut, SIGNAL(activated()), this, SLOT(hide()));
    QShortcut *helpShortcut = new QShortcut(QKeySequence("F1"), this);
    connect(helpShortcut, SIGNAL(activated()), this, SLOT(help()));
    QShortcut *restartShortcut = new QShortcut(QKeySequence("F2"), this);
    connect(restartShortcut, SIGNAL(activated()), this, SLOT(startMaryServer()));
    QShortcut *installDriversShortcut = new QShortcut(QKeySequence("F3"), this);
    connect(installDriversShortcut, SIGNAL(activated()), this, SLOT(installDrivers()));
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

    ui->okButton->setFocus();

    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(help()));
    connect(ui->restartButton, SIGNAL(clicked()), this, SLOT(startMaryServer()));
    connect(ui->installDriversButton, SIGNAL(clicked()), this, SLOT(installDrivers()));
    connect(ui->memoryButton, SIGNAL(clicked()), this, SLOT(displayMemoryStatus()));
    connect(ui->installDiskButton, SIGNAL(clicked()), this, SLOT(installDiskDrive()));
    connect(ui->enableClipboardButton, SIGNAL(clicked()), this, SLOT(enableClipBoard()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    connect(ui->exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    startMaryServer();
    //player = new Player(this);
    player = new Player();
    //player->hide();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(restartMaryServer()));
    timer->start(1000);

    clipBoard=QApplication::clipboard();
    connect(clipBoard, SIGNAL(dataChanged()), this, SLOT(clipBoardChanged()));
    IsclipBoardEnabled = false;
}

//Ok
MainWindow::~MainWindow()
{
    if (timer != NULL)
    {
        delete timer;
    }

    if (chooseDiskDialog != NULL)
    {
        delete chooseDiskDialog;
    }
    if (progressDialog != NULL)
    {
        delete progressDialog;
    }
    maryServerProcess.close();
    if ( player != NULL)
    {
        delete player;
    }
    delete ui;
}

//Ok
void MainWindow::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

//Ok
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

//Ok
void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("Ε&λαχιστοποίηση"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("Ε&παναφορά"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(restore()));

    quitAction = new QAction(tr("&Έξοδος"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

//Ok
void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

//Ok
void MainWindow::terminate()
{
    maryServerProcess.kill();
    qApp->quit();
}

//Ok
void MainWindow::restore()
{
    this->showNormal();
    this->raise();
    this->activateWindow();
    this->setFocus();
    ui->okButton->setFocus();
}

//Ok
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

//Ok
void MainWindow::startNVDA()
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;


    // Press the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Press the "ALT" key
    ip.ki.wVk = VK_MENU;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Press the "N" key
    ip.ki.wVk = 'N';
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "N" key
    ip.ki.wVk = 'N';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "ALT" key
    ip.ki.wVk = VK_MENU;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
}

//Ok
void MainWindow::startMaryServer()
{
    double availableMemory;
    memoryForMaryServer = 0;

    maryServerProcess.close();

    availableMemory = getAvailableMemory();

    //    //if there is enough memory available we allocate it to mary server
    //    //otherwise we choose the default allocation
    if (availableMemory > MAX_MEMORY_FOR_MARY)
    {
        //try to allocate 2Gb for mary server
        startMaryServerProcess(MAX_MEMORY_FOR_MARY);
    }

    //if process has not started try with 1gb
    if  (maryServerProcess.pid() == 0)
    {
        if (availableMemory > NORMAL_MEMORY_FOR_MARY)
        {
            //try to allocate 1Gb for mary server
            startMaryServerProcess(NORMAL_MEMORY_FOR_MARY);
        }
    }

    if  (maryServerProcess.pid() == 0)
    {
        //try to allocate 500mb for mary server
        startMaryServerProcess(MIN_MEMORY_FOR_MARY);
    }

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
    {
        startMaryServer();
    }
}

void MainWindow::startMaryServerProcess(int memory)
{
    double availableMemory;

    availableMemory = getAvailableMemory();

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

//Ok
void MainWindow::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

//Ok
void MainWindow::installDrivers()
{
    QString file1, file2;

    QString dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    file1 = dir + "\\AppData\\Roaming\\nvda\\synthDrivers\\openmary.py";
    file2 = dir + "\\AppData\\Roaming\\nvda\\synthDrivers\\openmary.pyo";

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
                QThread* thread = new QThread;
                Copy* copy = new Copy(drivePath);
                int maxFiles = copy->countMaxFiles();
                copy->moveToThread(thread);
                connect(copy, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
                connect(thread, SIGNAL(started()), copy, SLOT(process()));
                connect(copy, SIGNAL(finished()), thread, SLOT(quit()));
                connect(copy, SIGNAL(finished()), copy, SLOT(deleteLater()));
                connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
                connect(copy, SIGNAL(increase()), progressDialog, SLOT(increase()));
                connect(copy, SIGNAL(finished()), this, SLOT(installationComplete()));

                //Activate progress dialog
                //progressDialog->setWindowTitle("0% αντιγράφτηκε");
                progressDialog->resetProgressBar(maxFiles);
                progressDialog->show();
                progressDialog->raise();
                thread->start();
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

void MainWindow::clipBoardChanged()
{
    if (IsclipBoardEnabled)
    {
        QString text = clipBoard->text();
        player->speakClipBoardText(text);
    }
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
    player->setClipboardEnabled(IsclipBoardEnabled);
}

void MainWindow::stopPlayer()
{
    player->speakClipBoardText("");
}
