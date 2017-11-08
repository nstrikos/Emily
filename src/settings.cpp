#include "settings.h"

#include <QLocale>

Settings::Settings()
{
    //Sometimes nvda sends the first command for voice and rate too fast
    //and the application cannot receive them.
    //So we need a class to write and read voice and rate for the starting up
    m_settingsImpl = new SettingsImpl();
}

void Settings::setUpdater(SettingsIface *updater)
{
    m_settingsImpl->setUpdater(updater);
}

void Settings::readSettings()
{
    m_settingsImpl->readSettings();
}

void Settings::writeVoice(QString voice)
{
    m_settingsImpl->writeVoice(voice);
}

void Settings::writeRate(QString rate)
{
    m_settingsImpl->writeRate(rate);
}

void Settings::writeSettings()
{
    m_settingsImpl->writeSettings();
}

Settings::~Settings()
{
    delete m_settingsImpl;
}
