/*
 * Explanation: This class does nothing important, it delegates
 * the requests to SettingsImpl, which implements the functionality
*/

#include "settings.h"

Settings::Settings()
{
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

void Settings::writeSettings()
{
    m_settingsImpl->writeSettings();
}

void Settings::setVoice(QString voice)
{
    m_settingsImpl->setVoice(voice);
}

void Settings::setRate(QString rate)
{
    m_settingsImpl->setRate(rate);
}

QString Settings::voice()
{
    return m_settingsImpl->voice();
}

QString Settings::rate()
{
    return m_settingsImpl->rate();
}

Settings::~Settings()
{
    delete m_settingsImpl;
}
