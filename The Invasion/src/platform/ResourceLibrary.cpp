#include "platform/ResourceLibrary.h"
#include <allegro5/allegro_ttf.h>
#include <cstdio>
#include <string>

namespace {

/// Reports a missing asset and folds the failure into the running result.
template <typename T>
bool require(T* handle, const char* what)
{
    if (!handle)
    {
        std::fprintf(stderr, "ERROR: could not load %s\n", what);
        return false;
    }
    return true;
}

} // namespace

ResourceLibrary::~ResourceLibrary()
{
    for (ALLEGRO_BITMAP* frame : m_playerFrames) al_destroy_bitmap(frame);
    for (ALLEGRO_BITMAP* bitmap : m_enemyIdle) al_destroy_bitmap(bitmap);
    for (ALLEGRO_BITMAP* bitmap : m_enemyMoving) al_destroy_bitmap(bitmap);

    al_destroy_bitmap(m_withLife);
    al_destroy_bitmap(m_lifeless);
    al_destroy_bitmap(m_ammo);
    al_destroy_bitmap(m_noAmmo);
    al_destroy_bitmap(m_explosion);

    al_destroy_font(m_title);
    al_destroy_font(m_button);
    al_destroy_font(m_score);

    al_destroy_sample(m_battleMusic);
    al_destroy_sample(m_menuMusic);
    al_destroy_sample(m_beep);
}

bool ResourceLibrary::load()
{
    bool ok = true;

    for (int i = 0; i < PlayerFrames; ++i)
    {
        const std::string path = "Assets/Nave/nave" + std::to_string(i + 1) + ".png";
        m_playerFrames[i] = al_load_bitmap(path.c_str());
        ok &= require(m_playerFrames[i], path.c_str());
    }

    m_enemyIdle[0] = al_load_bitmap("Assets/inimigos.png");
    m_enemyIdle[1] = al_load_bitmap("Assets/inimigos2.png");
    m_enemyMoving[0] = al_load_bitmap("Assets/inimigos_ativo.png");
    m_enemyMoving[1] = al_load_bitmap("Assets/inimigos2_ativo.png");
    ok &= require(m_enemyIdle[0], "Assets/inimigos.png");
    ok &= require(m_enemyIdle[1], "Assets/inimigos2.png");
    ok &= require(m_enemyMoving[0], "Assets/inimigos_ativo.png");
    ok &= require(m_enemyMoving[1], "Assets/inimigos2_ativo.png");

    m_withLife = al_load_bitmap("Assets/com_vida.jpg");
    m_lifeless = al_load_bitmap("Assets/sem_vida.png");
    m_ammo = al_load_bitmap("Assets/missel.png");
    m_noAmmo = al_load_bitmap("Assets/sem_missel.png");
    m_explosion = al_load_bitmap("Assets/explosion.png");
    ok &= require(m_withLife, "Assets/com_vida.jpg");
    ok &= require(m_lifeless, "Assets/sem_vida.png");
    ok &= require(m_ammo, "Assets/missel.png");
    ok &= require(m_noAmmo, "Assets/sem_missel.png");
    ok &= require(m_explosion, "Assets/explosion.png");

    m_title = al_load_font("fonts/Title.otf", 100, 0);
    m_button = al_load_font("fonts/buttons.otf", 25, 0);
    m_score = al_load_font("fonts/Score.ttf", 30, 0);
    ok &= require(m_title, "fonts/Title.otf");
    ok &= require(m_button, "fonts/buttons.otf");
    ok &= require(m_score, "fonts/Score.ttf");

    m_battleMusic = al_load_sample("sounds/music.ogg");
    m_menuMusic = al_load_sample("sounds/menumusic.ogg");
    m_beep = al_load_sample("sounds/button_selected.wav");
    ok &= require(m_battleMusic, "sounds/music.ogg");
    ok &= require(m_menuMusic, "sounds/menumusic.ogg");
    ok &= require(m_beep, "sounds/button_selected.wav");

    return ok;
}

ALLEGRO_BITMAP* ResourceLibrary::playerFrame(int index) const
{
    if (index < 0 || index >= PlayerFrames)
        return m_playerFrames[0];
    return m_playerFrames[index];
}

ALLEGRO_BITMAP* ResourceLibrary::enemy(int type, bool moving) const
{
    const int slot = (type == 0) ? 0 : 1;
    return moving ? m_enemyMoving[slot] : m_enemyIdle[slot];
}
