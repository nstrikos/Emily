#include "player.h"

Player::Player()
{
    qDebug() << "Player constructor called";
    m_impl = new PlayerImpl();
}

void Player::setIndexHandler(PlayerIface *indexHandler)
{
    m_impl->setIndexHandler(indexHandler);
}

void Player::playFile(QBuffer* buffer, QString index)
{
    m_impl->playFile(buffer, index);
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
    qDebug() << "Player destructor called";
    delete m_impl;
}
