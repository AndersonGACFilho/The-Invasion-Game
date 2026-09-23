#pragma once

class Player;
class PlayerWeapon;
class EnemyWeapon;
class EnemyFleet;

/**
 * @brief Resolves interactions that belong to neither weapon alone.
 */
namespace CombatResolver
{
    /// Cancels out player shots and enemy shots that collide in mid-air.
    void resolveCrossfire(PlayerWeapon& playerWeapon,
                          EnemyWeapon& enemyWeapon,
                          const Player& player,
                          const EnemyFleet& fleet);
}
