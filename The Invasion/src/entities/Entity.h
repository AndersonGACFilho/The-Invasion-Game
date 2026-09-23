#pragma once

/**
 * @brief Base class for anything that occupies space in the world.
 *
 * Stores a centre position and a half-extent, and provides the symmetric
 * axis-aligned bounding box test.
 *
 * @note Only the player, enemy projectiles and projectile-versus-projectile
 *       checks use this symmetric test. Enemy hit detection and enemy
 *       crowding use asymmetric formulas that are deliberately kept in
 *       @ref Enemy and @ref EnemyFleet rather than forced into this class.
 */
class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    short int x() const { return m_x; }
    short int y() const { return m_y; }
    short int edgeX() const { return m_edgeX; }
    short int edgeY() const { return m_edgeY; }

    void setPosition(short int x, short int y) { m_x = x; m_y = y; }
    void setExtent(short int halfWidth, short int halfHeight)
    {
        m_edgeX = halfWidth;
        m_edgeY = halfHeight;
    }

    /// @return @c true when the two bounding boxes overlap.
    bool intersects(const Entity& other) const;

protected:
    short int m_x = 0;
    short int m_y = 0;
    short int m_edgeX = 0;
    short int m_edgeY = 0;
};
