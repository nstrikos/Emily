#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>
#include <QtWidgets>
#include <QClipboard>

class HotkeyThread : public QThread
 {
     Q_OBJECT
public:
    HotkeyThread();
    ~HotkeyThread();


protected:
    void run();

private:
    QClipboard *clipBoard;

signals:
     void setEmilyVoice();
     void setGoogleGreekVoice();
     void setEnglishVoice();
     void setHerculesVoice();
     void restoreWindow();
     void speakHighlightedText(QString);
     void stop();
 };

#endif // HOTKEYTHREAD_H
