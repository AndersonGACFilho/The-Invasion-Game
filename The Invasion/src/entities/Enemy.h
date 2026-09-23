#pragma once
#include "entities/Entity.h"

class Projectile;

/**
 * @brief A single enemy ship.
 *
 * @note The @c go_to_x and @c go_to_y fields of the original struct were
 *       never read or written anywhere in the code base and have been
 *       dropped.
 */
class Enemy : public Entity
{
public:
    /// Returns the ship to its off-screen, inactive state.
    void reset();

    /// Places the ship on screen and makes it active.
    void spawnAt(short int x, short int y, short int type);

    bool alive() const { return m_alive; }
    short int type() const { return m_type; }
    short int velocity() const { return m_velocity; }
    short int currentVelocity() const { return m_currentVelocity; }

    /// Stops the ship for this frame, used when another ship blocks it.
    void halt() { m_currentVelocity = 0; }
    void advance() { m_x -= m_currentVelocity; }

    /// Raises hit points and speed as the wave difficulty escalates.
    void toughen();

    /**
     * @brief Asymmetric hit test, preserved verbatim from the original.
     *
     * The left edge is offset by five pixels while the right edge uses the
     * ship's full extent, which makes the hitbox lean towards the nose of
     * the sprite.
     */
    bool isHitBy(const Projectile& shot) const;

    /**
     * @brief Applies damage.
     * @return @c true when this hit destroyed the ship.
     */
    bool absorb(short int damage);

private:
    short int m_life = 1;
    short int m_velocity = 3;
    short int m_currentVelocity = 3;
    short int m_type = 0;
    bool m_alive = false;
};
