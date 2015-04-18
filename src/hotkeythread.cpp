#include "hotkeythread.h"
#include "windows.h"
#include <QDebug>
#include <QApplication>
#define MOD_NOREPEAT    0x4000
#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002

HotkeyThread::HotkeyThread()
{
    clipBoard = QApplication::clipboard();
}

HotkeyThread::~HotkeyThread()
{
    if (clipBoard != NULL)
    {
        clipBoard = NULL;
    }
}

void HotkeyThread::run()
{
    RegisterHotKey(NULL,1,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x31); //Ctrl + Alt + 1
    RegisterHotKey(NULL,2,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x32); //Ctrl + Alt + 2
    RegisterHotKey(NULL,3,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x33); //Ctrl + Alt + 3
    RegisterHotKey(NULL,4,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x45); //Ctrl + Alt + E
    RegisterHotKey(NULL,5, MOD_CONTROL | MOD_NOREPEAT,0x70); //Ctrl + F1
    RegisterHotKey(NULL,6, MOD_CONTROL | MOD_NOREPEAT,0x71); //Ctrl + F2

    QApplication::processEvents();

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_HOTKEY){
            if (msg.wParam == 1)
                emit setGoogleGreekVoice();
            else if (msg.wParam == 2)
                emit setEnglishVoice();
            else if (msg.wParam == 3)
                emit setEmilyVoice();
            else if (msg.wParam == 4)
                emit restoreWindow();
            else if (msg.wParam == 5)
            {
                QString text = clipBoard->text();
                emit speakHighlightedText(text);
            }
            else if (msg.wParam == 6)
                emit stop();
        }
    }
}

