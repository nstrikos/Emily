#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include "choosediskdialog.h"
#include "copythread.h"
#include "progressdialog.h"
#include "constants.h"
#include <QtWidgets>
#include <QTimer>
#include <QClipboard>
#include <qxtglobalshortcut.h>

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
    void terminate();
    void restore();
    void startMaryServer();
    void restartMaryServer();
    void installDrivers();
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
    void clipBoardChanged();
    void enableClipBoard();
    void stopPlayer();

private:
    Ui::MainWindow *ui;
    Player *player;

    void createActions();
    void createTrayIcon();
    void startNVDA();
    void startMaryServerProcess(int memory);

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QString command;
    QProcess maryServerProcess;

    double getAvailableMemory();
    double getUsedMemory();
    void delay(int n);
    int memoryForMaryServer;

    QTimer *timer;
    ChooseDiskDialog *chooseDiskDialog;
    ProgressDialog *progressDialog;
    QClipboard *clipBoard;
    bool IsclipBoardEnabled;
};

#endif // MAINWINDOW_H
