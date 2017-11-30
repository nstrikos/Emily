/*
 * Class : SettingsImpl
 * Role : My responsibility is to implement Settings class
 * Collaborator : When I read settings I send them to m_settingsUpdater
 * which probably is a textMediator object
*/

#ifndef SETTINGSIMPL_H
#define SETTINGSIMPL_H

#include "settingsiface.h"
#include <QString>

class SettingsImpl
{
public:
    SettingsImpl();
    ~SettingsImpl();

    void setUpdater(SettingsIface *updater);
    void readSettings();
    void writeSettings();
    void setVoice(QString voice);
    void setRate(QString rate);
    QString voice();
    QString rate();

private:
    void readFromRegistry();
    void readFromCurFolder();
    void convertPreviousVersion();
    void writeToRegistry();
    void writeToCurFolder();
    void writeVoice(QString voice);
    void writeRate(QString rate);
    void initializeVoice();
    void initializeRate();
    QString m_voice;
    QString m_rate;
    SettingsIface *m_settingsUpdater;
};

#endif // SETTINGSIMPL_H
