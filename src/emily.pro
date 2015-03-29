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
    progressdialog.cpp \
    clipboardhandler.cpp

HEADERS  += mainwindow.h \
    downloadmanager.h \
    player.h \
    choosediskdialog.h \
    copythread.h \
    progressdialog.h \
    constants.h \
    clipboardhandler.h

FORMS    += mainwindow.ui \
    choosediskdialog.ui \
    progressdialog.ui

RESOURCES = emily.qrc
