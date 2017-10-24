#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>

class HotkeyThread : public QThread
 {
     Q_OBJECT
public:
    HotkeyThread();

protected:
    void run();

signals:
     void setGreekVoice();
     void setEnglishVoice();
     void restoreWindow();
};

#endif // HOTKEYTHREAD_H
