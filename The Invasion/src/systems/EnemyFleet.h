#pragma once
#include "entities/Enemy.h"
#include "core/Config.h"

class Player;
class ResourceLibrary;

/**
 * @brief Owns the enemy fleet: spawning, crowding, movement and drawing.
 */
class EnemyFleet
{
public:
    void reset();

    /**
     * @brief Advances the wave and may spawn one ship this tick.
     *
     * Every kill feeds the player's difficulty points; once they wrap, the
     * fleet grows. At twenty-five ships the wave resets to ten and the
     * survivors become tougher.
     */
    void spawn(Player& player);

    /// Moves every live ship, holding back those blocked by the ship ahead.
    void update();

    void draw(const ResourceLibrary& resources) const;

    short int count() const { return m_count; }
    short int aliveCount() const;

    Enemy& at(int index) { return m_enemies[index]; }
    const Enemy& at(int index) const { return m_enemies[index]; }

private:
    Enemy m_enemies[Config::MaxEnemies];
    short int m_count = 10;
};
