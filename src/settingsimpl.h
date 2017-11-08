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
    void writeVoice(QString voice);
    void writeRate(QString rate);
    void writeSettings();

private:
    void initializeVoice();
    void initializeRate();
    QString m_voice;
    QString m_rate;
    SettingsIface *m_settingsUpdater;
};

#endif // SETTINGSIMPL_H
