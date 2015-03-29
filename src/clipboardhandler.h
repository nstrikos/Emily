#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QtWidgets>
#include <QClipboard>

class ClipboardHandler : public QObject
{
     Q_OBJECT

public:
    ClipboardHandler();
    ~ClipboardHandler();
    void setEnabled(bool enabled);

private slots:
    void clipBoardChanged();

private:
    QClipboard *clipBoard;
    bool IsEnabled;

signals:
    void newClipBoardText(QString text);
};

#endif // CLIPBOARD_H
