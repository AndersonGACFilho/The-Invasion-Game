#include "systems/PlayerWeapon.h"
#include "entities/Player.h"
#include "systems/EnemyFleet.h"
#include <allegro5/allegro_primitives.h>

void PlayerWeapon::reset()
{
    for (Projectile& shot : m_shots)
        shot.configure(1, 10, 4);
}

void PlayerWeapon::fire(const Player& player)
{
    for (int i = 0; i < player.shots(); ++i)
    {
        if (!m_shots[i].active())
        {
            m_shots[i].launch(player.x() + 20, player.y());
            return;
        }
    }
}

void PlayerWeapon::update(Player& player, EnemyFleet& fleet)
{
    for (int i = 0; i < player.shots(); ++i)
    {
        Projectile& shot = m_shots[i];
        if (!shot.active())
            continue;

        shot.advance();
        if (shot.x() > Config::ScreenWidth)
            shot.deactivate();

        for (int j = 0; j < fleet.count(); ++j)
        {
            Enemy& enemy = fleet.at(j);
            if (!enemy.alive() || !enemy.isHitBy(shot))
                continue;

            // Original behaviour preserved: the shot is spent, yet the scan
            // continues, so a single shot can damage several enemies that
            // overlap within the same frame.
            shot.deactivate();
            if (enemy.absorb(shot.damage()))
                player.scoreKill();
        }
    }
}

void PlayerWeapon::draw(const Player& player) const
{
    for (int i = 0; i < player.shots(); ++i)
    {
        const Projectile& shot = m_shots[i];
        if (!shot.active())
            continue;

        al_draw_filled_rounded_rectangle(shot.x() + 10, shot.y() - 2,
                                         shot.x() - 2, shot.y() + 2,
                                         -10, -2, al_map_rgb(200, 255, 200));
        al_draw_filled_rectangle(shot.x() + 8, shot.y() - 1,
                                 shot.x() - 1, shot.y() + 1,
                                 al_map_rgb(0, 200, 0));
    }
}

short int PlayerWeapon::readyCount(const Player& player) const
{
    short int ready = 0;
    for (int i = 0; i < player.shots(); ++i)
        if (!m_shots[i].active())
            ready++;
    return ready;
}
