#include "systems/Hud.h"
#include "entities/Player.h"
#include "systems/PlayerWeapon.h"
#include "systems/EnemyFleet.h"
#include "platform/ResourceLibrary.h"
#include "core/Config.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

namespace {
constexpr int LifeIconX[3] = { 33, 63, 93 };
constexpr int AmmoIconX[3] = { 13, 43, 73 };
constexpr int LifeIconY = 8;
constexpr int AmmoIconY = 647;
}

void Hud::draw(const Player& player,
               const PlayerWeapon& weapon,
               const EnemyFleet& fleet,
               const ResourceLibrary& resources) const
{
    // Bottom and top frames.
    al_draw_filled_rectangle(0, 650, 1280, 720, al_map_rgb(70, 70, 70));
    al_draw_filled_rectangle(5, 655, 1275, 700, al_map_rgb(100, 100, 100));
    al_draw_filled_rectangle(0, 0, 1280, 60, al_map_rgb(70, 70, 70));
    al_draw_filled_rectangle(6, 6, 1274, 54, al_map_rgb(100, 100, 100));

    const short int lives = player.life();
    for (int i = 0; i < 3; ++i)
    {
        ALLEGRO_BITMAP* icon = (lives >= i + 1) ? resources.withLife()
                                                : resources.lifeless();
        al_draw_bitmap(icon, LifeIconX[i], LifeIconY, 0);
    }

    const short int ready = weapon.readyCount(player);
    for (int i = 0; i < 3; ++i)
    {
        ALLEGRO_BITMAP* icon = (ready > i) ? resources.ammo()
                                           : resources.noAmmo();
        al_draw_bitmap(icon, AmmoIconX[i], AmmoIconY, 0);
    }

    al_draw_textf(resources.buttonFont(), al_map_rgb(0, 200, 100),
                  Config::ScreenWidth - 145, 20, ALLEGRO_ALIGN_CENTER,
                  "Enemies Alive - %hd", fleet.aliveCount());
    al_draw_textf(resources.buttonFont(), al_map_rgb(0, 200, 200),
                  Config::ScreenWidth / 2, 20, ALLEGRO_ALIGN_CENTER,
                  "SCORE - %hd", player.score());
}
