#pragma once
#include "entities/Projectile.h"
#include "core/Config.h"

class Player;
class EnemyFleet;

/**
 * @brief Enemy guns: choosing who fires, travel, player hits and drawing.
 *
 * Each enemy owns the projectile slot matching its own index.
 */
class EnemyWeapon
{
public:
    void reset();

    /**
     * @brief Gives every unobstructed enemy a chance to fire.
     *
     * A ship holds fire while another ship shares its lane ahead of it, so
     * the fleet does not shoot itself in the back.
     */
    void fire(const Player& player, const EnemyFleet& fleet);

    /// Moves live projectiles and resolves hits against the player.
    void update(Player& player, const EnemyFleet& fleet);

    void draw(const EnemyFleet& fleet) const;

    Projectile& at(int index) { return m_missiles[index]; }
    const Projectile& at(int index) const { return m_missiles[index]; }

private:
    Projectile m_missiles[Config::MaxEnemies];
};
