#include "entities/Projectile.h"

void Projectile::configure(short int damage, short int velocity, short int extent)
{
    m_damage = damage;
    m_velocity = velocity;
    m_edgeX = extent;
    m_edgeY = extent;
    m_active = false;
}
