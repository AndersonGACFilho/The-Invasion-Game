#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

/**
 * @brief Sole owner of every asset loaded from disk.
 *
 * Follows RAII: the destructor releases everything.
 *
 * @note The original loaded the nine ship frames inside the gameplay loop,
 *       once per match, without ever destroying the previous set. The leak
 *       grew with every press of "play".
 */
class ResourceLibrary
{
public:
    ResourceLibrary() = default;
    ~ResourceLibrary();

    ResourceLibrary(const ResourceLibrary&) = delete;
    ResourceLibrary& operator=(const ResourceLibrary&) = delete;

    /**
     * @brief Loads every bitmap, font and sample.
     * @return @c false if any asset is missing.
     */
    bool load();

    /// @param index Animation frame, 0 to @ref PlayerFrames - 1.
    ALLEGRO_BITMAP* playerFrame(int index) const;

    /**
     * @param type   Enemy variant, 0 or 1.
     * @param moving Selects the thrusting sprite over the idle one.
     */
    ALLEGRO_BITMAP* enemy(int type, bool moving) const;

    ALLEGRO_BITMAP* withLife() const { return m_withLife; }
    ALLEGRO_BITMAP* lifeless() const { return m_lifeless; }
    ALLEGRO_BITMAP* ammo() const { return m_ammo; }
    ALLEGRO_BITMAP* noAmmo() const { return m_noAmmo; }
    ALLEGRO_BITMAP* explosion() const { return m_explosion; }

    ALLEGRO_FONT* titleFont() const { return m_title; }
    ALLEGRO_FONT* buttonFont() const { return m_button; }
    ALLEGRO_FONT* scoreFont() const { return m_score; }

    ALLEGRO_SAMPLE* battleMusic() const { return m_battleMusic; }
    ALLEGRO_SAMPLE* menuMusic() const { return m_menuMusic; }
    ALLEGRO_SAMPLE* beep() const { return m_beep; }

    /// Frames in the player ship animation.
    static constexpr int PlayerFrames = 9;

private:
    ALLEGRO_BITMAP* m_playerFrames[PlayerFrames] = {};
    ALLEGRO_BITMAP* m_enemyIdle[2] = {};
    ALLEGRO_BITMAP* m_enemyMoving[2] = {};

    ALLEGRO_BITMAP* m_withLife = nullptr;
    ALLEGRO_BITMAP* m_lifeless = nullptr;
    ALLEGRO_BITMAP* m_ammo = nullptr;
    ALLEGRO_BITMAP* m_noAmmo = nullptr;
    ALLEGRO_BITMAP* m_explosion = nullptr;

    ALLEGRO_FONT* m_title = nullptr;
    ALLEGRO_FONT* m_button = nullptr;
    ALLEGRO_FONT* m_score = nullptr;

    ALLEGRO_SAMPLE* m_battleMusic = nullptr;
    ALLEGRO_SAMPLE* m_menuMusic = nullptr;
    ALLEGRO_SAMPLE* m_beep = nullptr;
};
