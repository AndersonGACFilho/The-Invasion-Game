#include "entities/Player.h"
#include "core/Config.h"

void Player::reset()
{
    m_x = 40;
    m_y = Config::ScreenHeight / 2;
    m_edgeX = 20;
    m_edgeY = 9;
    m_shots = Config::ActivePlayerShots;
    m_life = 3;
    m_power = 0;
    m_points = 0;
    m_score = 0;
    m_velocity = 4;
}

void Player::moveUp()
{
    m_y -= m_velocity;
    if (m_y < 80)
        m_y = 80;
}

void Player::moveDown()
{
    m_y += m_velocity;
    if (m_y > Config::ScreenHeight - 90)
        m_y = Config::ScreenHeight - 90;
}

void Player::moveLeft()
{
    m_x -= m_velocity;
    if (m_x - m_edgeX < 12)
        m_x = m_edgeX + 12;
}

void Player::moveRight()
{
    // The player is confined to the left third of the screen.
    m_x += m_velocity;
    if (m_x > Config::ScreenWidth / 3 - 20)
        m_x = (Config::ScreenWidth / 3) - 20;
}
