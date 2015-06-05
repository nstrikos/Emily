#include "nvdacommandhandler.h"

NvdaCommandHandler::NvdaCommandHandler(Player* player)
{
    this->player = player;
}

void NvdaCommandHandler::handleCommand(QString command)
{
    if (command != "")
    {
        if (command.contains("Cancel"))
            player->stop();
        else if (command.contains("Pause"))
            player->pause();
        else if (command.contains("Start"))
            player->resume();
        else if (command.contains("Rate "))
        {
            command.replace("Rate ", "");
            player->setRate(command);
        }
    }
}

