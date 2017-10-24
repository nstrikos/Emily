#include "settings.h"

Settings::Settings()
{
    //Sometimes nvda sends the first command for voice and rate too fast
    //and the application cannot receive them.
    //So we need a class to write and read voice and rate for the starting up
    m_settingsUpdater = NULL;
}

void Settings::setUpdater(SettingsIface *updater)
{
    m_settingsUpdater = updater;
    read();
}

void Settings::read()
{
    qDebug() << "Reading saved settings...";

    QSettings settings("Emily", "Emily");
    m_voice = settings.value("Voice").toString();
    m_rate = settings.value("Rate").toString();

    qDebug() << "Saved voice: " << m_voice;
    qDebug() << "Saved rate:" << m_rate;

    if (m_voice == "")
        initializeVoice();
    if (m_rate == "")
        initializeRate();

    if (m_settingsUpdater != NULL)
    {
        m_settingsUpdater->updateVoice(m_voice);
        m_settingsUpdater->updateRate(m_rate);
    }
    else
        qDebug() << "Settings: I need to update settings, but there is nowhere I can send updated settings";


    //    This code helps to find the path of the settings file
    //    QString config_dir = QFileInfo(settings.fileName()).absolutePath() + "/";
    //    qDebug() << config_dir;
}

void Settings::writeVoice(QString voice)
{
    m_voice = voice;
    QSettings settings("Emily", "Emily");
    settings.setValue("Voice", m_voice);
    qDebug() << "Writing voice to settings: " << m_voice;
}

void Settings::writeRate(QString rate)
{
    rate = rate.trimmed();
    m_rate = rate;
    QSettings settings("Emily", "Emily");
    settings.setValue("Rate", m_rate);
    qDebug() << "Writing rate to settings: " << m_rate;
}

void Settings::writeSettings()
{
    writeVoice(m_voice);
    writeRate(m_rate);
}

void Settings::initializeVoice()
{
    qDebug() << "Empty voice found.";
    qDebug() << "Initializing voice...";
    QString systemLanguage = QLocale::languageToString(QLocale::system().language());
    qDebug() << "System language is: " << systemLanguage;

    if (systemLanguage == QLocale::languageToString(QLocale::Greek))
        m_voice = herculesVoiceDisplay;
    else if (systemLanguage == QLocale::languageToString(QLocale::English))
    {
        QLocale locale = QLocale::system();
        if (locale.country() == QLocale::UnitedStates)
            m_voice = rmsVoiceDisplay;
        else
            m_voice = spikeVoiceDisplay;
    }
    else if (systemLanguage == QLocale::languageToString(QLocale::German))
        m_voice = pavoqueVoiceDisplay;
    else if (systemLanguage == QLocale::languageToString(QLocale::Turkish))
        m_voice = turkishVoiceDisplay;
    else if (systemLanguage == QLocale::languageToString(QLocale::French))
        m_voice = pierreVoiceDisplay;
    else if (systemLanguage == QLocale::languageToString(QLocale::Italian))
        m_voice = luciaVoiceDisplay;
    else if (systemLanguage == QLocale::languageToString(QLocale::Telugu))
        m_voice = teluguVoiceDisplay;
    else
        m_voice = herculesVoiceDisplay;

    qDebug() << "Voice initialized to: " << m_voice;
}

void Settings::initializeRate()
{
    qDebug() << "Empty rate found.";
    m_rate = "50";
    qDebug() << "Rate initialized to: " << m_rate;
}
