#pragma once
#include "entities/Entity.h"

/**
 * @brief A single shot, fired either by the player or by an enemy.
 *
 * Corresponds to the @c Powers struct of the original implementation,
 * which served both roles.
 */
class Projectile : public Entity
{
public:
    /**
     * @brief Sets the immutable traits of this projectile.
     * @param damage   Hit points removed from whatever it strikes.
     * @param velocity Pixels travelled per tick.
     * @param extent   Half-extent used for collision, on both axes.
     */
    void configure(short int damage, short int velocity, short int extent);

    void launch(short int x, short int y)
    {
        m_x = x;
        m_y = y;
        m_active = true;
    }

    void deactivate() { m_active = false; }
    bool active() const { return m_active; }

    /// Moves right, towards the enemies.
    void advance() { m_x += m_velocity; }
    /// Moves left, towards the player.
    void retreat() { m_x -= m_velocity; }

    short int damage() const { return m_damage; }

private:
    short int m_damage = 1;
    short int m_velocity = 0;
    bool m_active = false;
};
