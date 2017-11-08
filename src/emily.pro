#-------------------------------------------------
#
# Project created by QtCreator 2013-05-10T11:21:36
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network widgets

TARGET = emily
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    downloadmanager.cpp \
    player.cpp \
    choosediskdialog.cpp \
    copythread.cpp \
    progressdialog.cpp \
    hotkeythread.cpp \
    nvdatexthandler.cpp \
    nvdasender.cpp \
    selectvoicedialog.cpp \
    solution.cpp \
    settings.cpp \
    nvdacommandreceiver.cpp \
    textmediator.cpp \
    commandmediator.cpp \
    nvdatextreceiver.cpp \
    downloadmanagerimpl.cpp \
    playerimpl.cpp \
    settingsimpl.cpp

HEADERS  += mainwindow.h \
    downloadmanager.h \
    player.h \
    choosediskdialog.h \
    copythread.h \
    progressdialog.h \
    constants.h \
    hotkeythread.h \
    nvdasender.h \
    selectvoicedialog.h \
    solution.h \
    settings.h \
    nvdacommandreceiver.h \
    textmediator.h \
    commandmediator.h \
    nvdatexthandler.h \
    nvdatextreceiver.h \
    nvdatextreceiveriface.h \
    nvdatexthandleriface.h \
    downloadmanageriface.h \
    playeriface.h \
    nvdacommandreceiveriface.h \
    settingsiface.h \
    downloadmanagerimpl.h \
    playerimpl.h \
    settingsimpl.h

FORMS    += mainwindow.ui \
    choosediskdialog.ui \
    progressdialog.ui \
    selectvoicedialog.ui

win32:LIBS += -lpsapi

RESOURCES = emily.qrc

RC_ICONS = sound.ico
