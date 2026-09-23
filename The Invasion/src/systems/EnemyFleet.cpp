#include "systems/EnemyFleet.h"
#include "entities/Player.h"
#include "platform/ResourceLibrary.h"
#include <allegro5/allegro.h>
#include <cstdlib>

void EnemyFleet::reset()
{
    for (Enemy& enemy : m_enemies)
        enemy.reset();
    m_count = 10;
}

short int EnemyFleet::aliveCount() const
{
    short int alive = 0;
    for (int i = 0; i < m_count; ++i)
        if (m_enemies[i].alive())
            alive++;
    return alive;
}

void EnemyFleet::spawn(Player& player)
{
    const int chance = 500;

    // Operator precedence preserved from the original: (points % count) * 2.
    if (player.points() % m_count * 2 == 0 && player.points())
    {
        player.clearPoints();
        m_count++;
    }

    for (int i = 0; i < m_count; ++i)
    {
        // Reaching twenty-five ships rolls the wave back to ten and
        // escalates difficulty instead.
        if (m_count == 25)
        {
            m_count = 10;
            m_enemies[i].toughen();
            player.gainLife();
        }

        if (!m_enemies[i].alive() && rand() % chance == 0)
        {
            // Seven fixed lanes, ninety pixels apart.
            m_enemies[i].spawnAt(Config::ScreenWidth,
                                 93 + (rand() % 7) * 90,
                                 rand() % 2);
            break;
        }
    }
}

void EnemyFleet::update()
{
    for (int i = 0; i < m_count; ++i)
    {
        Enemy& self = m_enemies[i];
        if (!self.alive())
            continue;

        // A trailing ship stops rather than overlapping the one ahead.
        for (int j = 0; j < m_count; ++j)
        {
            if (j == i || !m_enemies[j].alive())
                continue;

            Enemy& other = m_enemies[j];
            const bool overlapping =
                self.x() + self.edgeX() * 2 - 7 > other.x() &&
                self.y() + self.edgeY() > other.y() &&
                self.x() < other.x() + other.edgeX() * 2 - 7 &&
                self.y() < other.y() + other.edgeX();

            if (overlapping)
            {
                if (self.x() > other.x())
                    self.halt();
                else
                    other.halt();
            }
        }

        // Ships halt once they reach the player's third of the screen.
        if (self.x() > (Config::ScreenWidth / 3) + 20)
            self.advance();
        else
            self.halt();
    }
}

void EnemyFleet::draw(const ResourceLibrary& resources) const
{
    for (int i = 0; i < m_count; ++i)
    {
        const Enemy& enemy = m_enemies[i];
        if (!enemy.alive())
            continue;

        const bool moving = enemy.currentVelocity() != 0;
        al_draw_bitmap(resources.enemy(enemy.type(), moving),
                       enemy.x(), enemy.y() - 44, 0);
    }
}
