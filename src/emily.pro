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
    hotkeythread.cpp \
    nvdareceiver.cpp \
    nvdatexthandler.cpp \
    indextextlist.cpp \
    nvdacommandhandler.cpp \
    nvdasender.cpp \
    selectvoicedialog.cpp

HEADERS  += mainwindow.h \
    downloadmanager.h \
    player.h \
    choosediskdialog.h \
    copythread.h \
    progressdialog.h \
    constants.h \
    hotkeythread.h \
    nvdareceiver.h \
    nvdatexthandler.h \
    indextextlist.h \
    nvdacommandhandler.h \
    nvdasender.h \
    selectvoicedialog.h

FORMS    += mainwindow.ui \
    choosediskdialog.ui \
    progressdialog.ui \
    selectvoicedialog.ui

TRANSLATIONS = emily_en.ts \
               emily_el.ts

win32:LIBS += -lpsapi

RESOURCES = emily.qrc \
    translations.qrc
