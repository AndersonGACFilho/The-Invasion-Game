#pragma once
#include "entities/Projectile.h"
#include "core/Config.h"

class Player;
class EnemyFleet;

/**
 * @brief The player's guns: firing, travel, enemy hits and drawing.
 */
class PlayerWeapon
{
public:
    void reset();

    /// Launches the first free projectile, if any.
    void fire(const Player& player);

    /// Moves live shots and resolves their hits against the fleet.
    void update(Player& player, EnemyFleet& fleet);

    void draw(const Player& player) const;

    /// @return How many shots are still loaded, i.e. not in flight.
    short int readyCount(const Player& player) const;

    Projectile& at(int index) { return m_shots[index]; }
    const Projectile& at(int index) const { return m_shots[index]; }

private:
    Projectile m_shots[Config::MaxPlayerShots];
};
