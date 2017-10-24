#include "hotkeythread.h"
#include "windows.h"
#include <QApplication>
#define MOD_NOREPEAT    0x4000
#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002

HotkeyThread::HotkeyThread()
{

}

void HotkeyThread::run()
{
    RegisterHotKey(NULL,1,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x31); //Ctrl + Alt + 1
    RegisterHotKey(NULL,2,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x32); //Ctrl + Alt + 2
    RegisterHotKey(NULL,3,MOD_ALT | MOD_CONTROL | MOD_NOREPEAT,0x45); //Ctrl + Alt + E

    QApplication::processEvents();

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_HOTKEY){
            if (msg.wParam == 1)
                emit setEnglishVoice();
            else if (msg.wParam == 2)
                emit setGreekVoice();
            else if (msg.wParam == 3)
                emit restoreWindow();
        }
    }
}

