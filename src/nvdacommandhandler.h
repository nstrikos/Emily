#ifndef NVDACOMMANDHANDLER_H
#define NVDACOMMANDHANDLER_H

#include <QObject>
#include <player.h>

class NvdaCommandHandler : public QObject
{
    Q_OBJECT
public:
    NvdaCommandHandler(Player* player);
    void handleCommand(QString command);

private:
    Player* player;

signals:

public slots:
};

#endif // NVDACOMMANDHANDLER_H
