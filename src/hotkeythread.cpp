#include "hotkeythread.h"
#include "windows.h"
#include <QDebug>
#include <QApplication>
#define MOD_NOREPEAT    0x4000
#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002

HotkeyThread::HotkeyThread()
{

}

HotkeyThread::~HotkeyThread()
{

}

void HotkeyThread::run()
{
    RegisterHotKey(NULL,1,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x31);
    RegisterHotKey(NULL,2,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x32);
    RegisterHotKey(NULL,3,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x33);
    RegisterHotKey(NULL,4,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x45);

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
        }
    }
}

