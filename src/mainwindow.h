#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "choosediskdialog.h"
#include "copythread.h"
#include "progressdialog.h"
#include "constants.h"
#include <windows.h>
#include <psapi.h>
#include <QtWidgets>
#include <QTimer>
#include "hotkeythread.h"
#include "nvdareceiver.h"
#include "indextextlist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void restore();
    void startMaryServer();
    void restartMaryServer();
    void installAddon();
    void displayMemoryStatus();
    void help();
    void about();
    void setEnglishVoice();
    void setGoogleGreekVoice();
    void setEmilyVoice();
    void installDiskDrive();
    void installationComplete();
    void increaseRate();
    void decreaseRate();
    void stopPlayer();
    void test();

private:
    Ui::MainWindow *ui;
    Player *player;

    void createActions();
    void createTrayAndIcons();
    void createAndInitializeObjects();
    void createShortcuts();
    void createConnections();
    void startMaryServerProcess(int memory);
    double getAvailableMemory();
    double getUsedMemory();
    void delay(int n);
    void writeSettings();
    void readSettings();
    void setVoice(QString voice);
    //void startNVDA();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QString command;
    QProcess maryServerProcess;
    int memoryForMaryServer;

    QTimer *timer;
    ChooseDiskDialog *chooseDiskDialog;
    ProgressDialog *progressDialog;
    HotkeyThread hotkeyThread;
    QString voice;

    NvdaReceiver *nvdaReceiver;
    IndexTextList *indexTextList;
};

#endif // MAINWINDOW_H
