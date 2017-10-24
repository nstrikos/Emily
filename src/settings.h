#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDebug>

#include "constants.h"
#include "settingsiface.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    Settings();

public:
    void setUpdater(SettingsIface *updater);
    void read();

public slots:
    void writeVoice(QString voice);
    void writeRate(QString rate);
    void writeSettings();

signals:
    void updateVoice(QString voice);
    void updateRate(QString rate);

private:
    void initializeVoice();
    void initializeRate();
    QString m_voice;
    QString m_rate;
    SettingsIface *m_settingsUpdater;
};

#endif // SETTINGS_H
