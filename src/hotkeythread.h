#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>

class HotkeyThread : public QThread
 {
     Q_OBJECT
public:
    HotkeyThread();
    ~HotkeyThread();


 protected:
     void run();

signals:
     void setEmilyVoice();
     void setGoogleGreekVoice();
     void setEnglishVoice();
     void restoreWindow();
 };

#endif // HOTKEYTHREAD_H
