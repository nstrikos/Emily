/*
 * Class : Player
 * Role : Player is responsible for playing media buffers.
 * When a buffer is played, player will send its index to nvda
 * Call playFile function to add a buffer media to the list of the player.
 * Player is responsible for deleting the buffers and freeing memory
 * Players also controls the rate of the mediaplayer, and can stop, pause and restart
 * the media player
 * Collaborators : When a wav file is played its index is send to m_indexHandler
 * which probably is a textMediator object
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QBuffer>
#include "playerimpl.h"
#include "playeriface.h"

class Player : public QObject
{
    Q_OBJECT

public:
    Player();
    ~Player();
    void setIndexHandler(PlayerIface *indexHandler);
    void playFile(QBuffer* buffer, QString index);
    void setRate(QString rateString);
    void stop();
    void resume();
    void pause();

private:
    PlayerImpl *m_impl;

};

#endif // PLAYER_H
