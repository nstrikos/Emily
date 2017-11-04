/*
 * Explanation: This class does nothing important, it delegates
 * the requests to PlayerImpl, which implements the functionality
*/

#include "player.h"

Player::Player()
{
    m_impl = new PlayerImpl();
}

void Player::setIndexHandler(PlayerIface *indexHandler)
{
    m_impl->setIndexHandler(indexHandler);
}

void Player::addPlaylist(QBuffer* buffer, QString index)
{
    m_impl->addPlaylist(buffer, index);
}

void Player::stop()
{
    m_impl->stop();
}

void Player::resume()
{
    m_impl->resume();
}

void Player::pause()
{
    m_impl->pause();
}

void Player::setRate(QString rateString)
{
    m_impl->setRate(rateString);
}

Player::~Player()
{
    delete m_impl;
}
