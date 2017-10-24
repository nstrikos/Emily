#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "choosediskdialog.h"
#include "copythread.h"
#include "progressdialog.h"
#include "constants.h"
#include <windows.h>
#include <psapi.h>
#include <QtWidgets>
#include <QTimer>
#include "hotkeythread.h"
#include "selectvoicedialog.h"

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

public slots:
    void quitReceived();

signals:
    void writeSettings();

private slots:
    void restore();
    void startMaryServer();
    void restartMaryServer();
    void installAddon();
    void displayMemoryStatus();
    void help();
    void selectVoice();
    void about();
    void setEnglishVoice();
    void setGreekVoice();
    void installDiskDrive();
    void installationComplete();
    //void voiceChanged(QString voice);

private:
    Ui::MainWindow *ui;

    void createActions();
    void createTrayAndIcons();
    void createAndInitializeObjects();
    void createShortcuts();
    void createConnections();
    void startMaryServerProcess(int memory);
    double getAvailableMemory();
    double getUsedMemory();
    void delay(int n);
    //void startNVDA();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *restoreAction;

    QString command;
    QProcess maryServerProcess;
    int memoryForMaryServer;

    QTimer *timer;
    ChooseDiskDialog *chooseDiskDialog;
    ProgressDialog *progressDialog;
    HotkeyThread hotkeyThread;
    QString voice;

    SelectVoiceDialog *selectVoiceDialog;

    QShortcut *okShortcut;
    QShortcut *helpShortcut;
    QShortcut *selectVoiceShortcut;
    QShortcut *restartShortcut;
    QShortcut *installAddonShortcut;
    QShortcut *memoryShortcut;
    QShortcut *installDiskDriveShortcut;
    QShortcut *aboutShortcut;
};

#endif // MAINWINDOW_H
