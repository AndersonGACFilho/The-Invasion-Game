#include "systems/CombatResolver.h"
#include "systems/PlayerWeapon.h"
#include "systems/EnemyWeapon.h"
#include "systems/EnemyFleet.h"
#include "entities/Player.h"

void CombatResolver::resolveCrossfire(PlayerWeapon& playerWeapon,
                                      EnemyWeapon& enemyWeapon,
                                      const Player& player,
                                      const EnemyFleet& fleet)
{
    for (int i = 0; i < fleet.count(); ++i)
    {
        Projectile& incoming = enemyWeapon.at(i);
        if (!incoming.active())
            continue;

        for (int j = 0; j < player.shots(); ++j)
        {
            Projectile& outgoing = playerWeapon.at(j);
            if (!outgoing.active())
                continue;

            if (incoming.intersects(outgoing))
            {
                incoming.deactivate();
                outgoing.deactivate();
            }
        }
    }
}
