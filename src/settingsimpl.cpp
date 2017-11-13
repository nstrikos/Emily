#include "settingsimpl.h"

#include <QLocale>
#include <QSettings>
#include <QDir>
#include <QDebug>

#include "constants.h"

SettingsImpl::SettingsImpl()
{
    m_settingsUpdater = NULL;
}

void SettingsImpl::setUpdater(SettingsIface *updater)
{
    m_settingsUpdater = updater;
}

void SettingsImpl::readSettings()
{
    QString dir =  QDir::currentPath();

    if (dir.startsWith("c", Qt::CaseInsensitive))
        readFromRegistry();
    else
        readFromCurFolder();

    if (m_voice == "")
        initializeVoice();
    if (m_rate == "")
        initializeRate();

    if (m_settingsUpdater != NULL)
    {
        m_settingsUpdater->updateVoice(m_voice);
        m_settingsUpdater->updateRate(m_rate);
    }
}

void SettingsImpl::readFromRegistry()
{
    QSettings settings("Emily", "Emily");
    m_voice = settings.value("Voice").toString();
    m_rate = settings.value("Rate").toString();

    //    This code helps to find the path of the settings file
    //    QString config_dir = QFileInfo(settings.fileName()).absolutePath() + "/";
    //    qDebug() << config_dir;
    qDebug() << m_voice << " " << m_rate;
}

void SettingsImpl::readFromCurFolder()
{
    QString text;
    QString filename = QDir::currentPath() + "/userSettings";
    QFile file( filename );
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        text = in.readAll();

        int i = text.indexOf(":");
        int l = text.indexOf("\r");
        m_voice = text.mid(i + 2, l - i - 2);
        text = text.right(l);
        i = text.indexOf(":");
        l = text.indexOf("\r");
        m_rate = text.mid(i + 2, l - i - 2);
        qDebug() << m_voice;
        qDebug() << m_rate;
    }

    file.close();
}

void SettingsImpl::writeSettings()
{
    QString dir =  QDir::currentPath();

    if (dir.startsWith("c", Qt::CaseInsensitive))
        writeToRegistry();
    else
        writeToCurFolder();
}

void SettingsImpl::writeToRegistry()
{
    QSettings settings("Emily", "Emily");
    settings.setValue("Voice", m_voice);
    settings.setValue("Rate", m_rate);
}

void SettingsImpl::writeToCurFolder()
{
    QString filename = QDir::currentPath() + "/userSettings";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Text) )
    {
        file.resize(0);
        QTextStream stream( &file );
        stream << "Voice: " << m_voice << endl;
        stream << "Rate: " << m_rate;
    }
    file.close();
}

void SettingsImpl::setVoice(QString voice)
{
    m_voice = voice;
}

void SettingsImpl::setRate(QString rate)
{
    m_rate = rate.trimmed();
}

QString SettingsImpl::voice()
{
    return m_voice;
}

QString SettingsImpl::rate()
{
    return m_rate;
}

void SettingsImpl::initializeVoice()
{
    QString systemLanguage = QLocale::languageToString(QLocale::system().language());

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
}

void SettingsImpl::initializeRate()
{
    m_rate = "50";
}

SettingsImpl::~SettingsImpl()
{

}
