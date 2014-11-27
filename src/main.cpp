#include "mainwindow.h"
#include "player.h"

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

    //    //---- Check for another instance code snippet ----

    //    //GUID : Generated once for your application
    //    // you could get one GUID here: http://www.guidgenerator.com/online-guid-generator.aspx
    QSharedMemory sharedMemory;
    sharedMemory.setKey("b1387fec-5d5c-41ee-a35e-ae473e3a4462");
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

    MainWindow w;

    //we start the application minimized
    //w.show();

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
