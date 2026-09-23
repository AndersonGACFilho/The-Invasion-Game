#pragma once
#include "entities/Entity.h"

/**
 * @brief The player ship: its state and its movement rules.
 *
 * Screen clamping belongs to the player, so the bounds live here rather
 * than in the scene that drives it.
 */
class Player : public Entity
{
public:
    /// Restores the starting position, lives, ammunition and score.
    void reset();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    short int life() const { return m_life; }
    short int shots() const { return m_shots; }
    short int points() const { return m_points; }
    short int score() const { return m_score; }

    void takeHit() { m_life -= 1; }
    /// Grants an extra life, never exceeding the maximum of three.
    void gainLife() { if (m_life < 3) m_life++; }
    void clearPoints() { m_points = 0; }
    /// Registers a kill: one difficulty point and ten score points.
    void scoreKill() { m_points++; m_score += 10; }
    bool isDead() const { return m_life < 1; }

private:
    short int m_life = 3;
    short int m_power = 0;
    short int m_shots = 3;
    short int m_points = 0;
    short int m_score = 0;
    short int m_velocity = 4;
};
