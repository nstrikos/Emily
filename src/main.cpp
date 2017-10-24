#include "mainwindow.h"
#include "solution.h"

#ifndef QT_NO_SYSTEMTRAYICON

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(0, QObject::tr("Systray"),
        QObject::tr("Η εφαρμογή δεν μπόρεσε να βρει εργαλειοθήκη συστήματος."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    //---- Check for another instance code snippet ----
    // key must be unique for the application
    QSharedMemory sharedMemory;
    sharedMemory.setKey("Emily - Open Mary voices for NVDA");
    if(sharedMemory.attach())
    {
        ;
        //Do nothing;
    }

    if (!sharedMemory.create(1))
    {
        QMessageBox msgBox;
        msgBox.setText( QObject::tr("Η εφαρμογή έχει ήδη ξεκινήσει.") );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
        qWarning() << "Can't start more than one instance of the application.";

        exit(0);
    }
    //---- END OF Check for another instance code snippet ----

    Solution solution;

    return app.exec();
}

#else

#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString text(tr("Το QSystemTrayIcon δεν υποστηρίζεται σε αυτό το λειτουργικό σύστημα"));

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);
    label->show();
    app.exec();
}

#endif
