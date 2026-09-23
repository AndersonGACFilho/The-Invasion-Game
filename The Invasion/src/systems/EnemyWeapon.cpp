#include "systems/EnemyWeapon.h"
#include "entities/Player.h"
#include "systems/EnemyFleet.h"
#include <allegro5/allegro_primitives.h>
#include <cstdlib>

void EnemyWeapon::reset()
{
    for (Projectile& missile : m_missiles)
        missile.configure(1, 7, 4);
}

void EnemyWeapon::fire(const Player& player, const EnemyFleet& fleet)
{
    const short int count = fleet.count();

    short int shipsAhead[Config::MaxEnemies] = {};
    for (int i = 0; i < count; ++i)
    {
        if (!fleet.at(i).alive())
            continue;

        for (int j = 0; j < count; ++j)
        {
            if (j == i || !fleet.at(j).alive())
                continue;
            if (fleet.at(i).y() == fleet.at(j).y() && fleet.at(i).x() > fleet.at(j).x())
                shipsAhead[i]++;
        }
    }

    // The odds tighten as the score climbs, making enemies fire more often.
    //
    // The original had no floor here. Once the score was high enough the
    // expression reached zero and `rand() % chance` divided by zero, which
    // crashed the game outright.
    int chance = 500 - 50 * player.score() / count / 10;
    if (chance < 1)
        chance = 1;

    for (int attempt = 0; attempt < count; ++attempt)
    {
        const int i = rand() % count;
        if (shipsAhead[i] != 0 || !fleet.at(i).alive() || m_missiles[i].active())
            continue;

        if (rand() % chance == 0)
            m_missiles[i].launch(fleet.at(i).x(), fleet.at(i).y());
    }
}

void EnemyWeapon::update(Player& player, const EnemyFleet& fleet)
{
    for (int i = 0; i < fleet.count(); ++i)
    {
        Projectile& missile = m_missiles[i];

        if (missile.active())
        {
            missile.retreat();
            if (missile.intersects(player))
            {
                missile.deactivate();
                player.takeHit();
            }
        }

        if (missile.x() < 0)
            missile.deactivate();
    }
}

void EnemyWeapon::draw(const EnemyFleet& fleet) const
{
    for (int i = 0; i < fleet.count(); ++i)
    {
        const Projectile& missile = m_missiles[i];
        if (!missile.active())
            continue;

        al_draw_filled_rectangle(missile.x() - 2, missile.y() - 2,
                                 missile.x() + 2, missile.y() + 2,
                                 al_map_rgb(255, 200, 200));
        al_draw_filled_rectangle(missile.x() - 1, missile.y() - 1,
                                 missile.x() + 1, missile.y() + 1,
                                 al_map_rgb(255, 200, 200));
    }
}
