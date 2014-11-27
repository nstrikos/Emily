#-------------------------------------------------
#
# Project created by QtCreator 2013-05-10T11:21:36
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = emily
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    downloadmanager.cpp \
    player.cpp \
    choosediskdialog.cpp \
    copythread.cpp \
    progressdialog.cpp

HEADERS  += mainwindow.h \
    downloadmanager.h \
    player.h \
    choosediskdialog.h \
    copythread.h \
    progressdialog.h \
    constants.h

FORMS    += mainwindow.ui \
    choosediskdialog.ui \
    progressdialog.ui

RESOURCES = emily.qrc


win32:CONFIG(release, debug|release): LIBS += -LC:/Qt/libqxt-qt5.2/lib/ -lqxtcore -lpsapi
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Qt/libqxt-qt5.2/lib/ -lqxtcore -lpsapi
win32:CONFIG(release, debug|release): LIBS += -LC:/Qt/libqxt-qt5.2/lib/ -lqxtwidgets -lpsapi
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Qt/libqxt-qt5.2/lib/ -lqxtwidgets -lpsapi

INCLUDEPATH += C:/Qt/libqxt-qt5.2/src/core
DEPENDPATH += C:/Qt/libqxt-qt5.2/src/core
INCLUDEPATH += C:/Qt/libqxt-qt5.2/src/widgets
DEPENDPATH += C:/Qt/libqxt-qt5.2/src/widgets

CONFIG  += qxt
QXT     += core gui
