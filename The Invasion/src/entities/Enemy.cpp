#include "entities/Enemy.h"
#include "entities/Projectile.h"
#include "core/Config.h"

void Enemy::reset()
{
    m_x = Config::ScreenWidth + 20;
    m_edgeX = 60;
    m_edgeY = 13;
    m_life = 1;
    m_alive = false;
    m_velocity = 3;
    m_currentVelocity = 3;
}

void Enemy::spawnAt(short int x, short int y, short int type)
{
    m_x = x;
    m_y = y;
    m_type = type;
    m_alive = true;
}

void Enemy::toughen()
{
    m_life++;
    if (m_velocity < 9)
        m_velocity++;
}

bool Enemy::isHitBy(const Projectile& shot) const
{
    return shot.x() + shot.edgeX() > m_x + 5
        && shot.x() - shot.edgeX() < m_x + m_edgeX
        && shot.y() + shot.edgeY() > m_y - m_edgeY
        && shot.y() - shot.edgeY() < m_y + m_edgeY;
}

bool Enemy::absorb(short int damage)
{
    m_life -= damage;
    if (m_life > 0)
        return false;

    m_alive = false;
    m_currentVelocity = m_velocity;
    return true;
}
