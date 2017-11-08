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
    void writeVoice(QString voice);
    void writeRate(QString rate);
    void writeSettings();

private:
    SettingsImpl *m_settingsImpl;
};

#endif // SETTINGS_H
