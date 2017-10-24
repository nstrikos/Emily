#ifndef SETTINGSIFACE_H
#define SETTINGSIFACE_H

class QString;

class SettingsIface
{
public:
    virtual void updateVoice(QString voice) = 0;
    virtual void updateRate(QString rate) = 0;
};

#endif // SETTINGSIFACE_H
