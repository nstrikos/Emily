/*
 * Class : Settings
 * Role : My responsibility is to read and write user settings (voice, rate)
 * so the user does not have to set them again.
 * Sometimes nvda sends the first command for voice and rate too fast
 * and the application cannot receive them.
 * So we need a class to write and read voice and rate for the starting up.
 * I am also responsible for initializing settings for the first time.
 * Collaborator : When I read settings I send them to m_settingsUpdater
 * which probably is a commandMediator object
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include "settingsiface.h"
#include "settingsimpl.h"

class Settings
{

public:
    Settings();
    ~Settings();

    void setUpdater(SettingsIface *updater);
    void readSettings();
    void writeSettings();
    void setVoice(QString voice);
    void setRate(QString rate);
    QString voice();
    QString rate();

private:
    SettingsImpl *m_settingsImpl;
};

#endif // SETTINGS_H
