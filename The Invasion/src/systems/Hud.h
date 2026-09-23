#pragma once

class Player;
class PlayerWeapon;
class EnemyFleet;
class ResourceLibrary;

/**
 * @brief Draws the in-match interface.
 *
 * Covers the top and bottom frames, the life and ammunition indicators,
 * the score and the live enemy count.
 */
class Hud
{
public:
    void draw(const Player& player,
              const PlayerWeapon& weapon,
              const EnemyFleet& fleet,
              const ResourceLibrary& resources) const;
};
