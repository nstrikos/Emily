#ifndef SETTINGSSTORAGEIFACE_H
#define SETTINGSSTORAGEIFACE_H

class QString;

class SettingsStorageIface
{
public:
    virtual QString getVoice() = 0;
    virtual QString getRate() = 0;
};

#endif // SETTINGSSTORAGEIFACE_H
