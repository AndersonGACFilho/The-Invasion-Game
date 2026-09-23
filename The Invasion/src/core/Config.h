#pragma once

/**
 * @file Config.h
 * @brief World-wide tuning constants.
 *
 * These values were scattered as @c #define directives and magic numbers
 * throughout the original single-file implementation.
 */
namespace Config
{
    /// Display width in pixels.
    constexpr int ScreenWidth = 1280;
    /// Display height in pixels.
    constexpr int ScreenHeight = 720;
    /// Simulation ticks per second.
    constexpr int Fps = 75;

    /**
     * @brief Slots reserved for player projectiles.
     *
     * Only @ref ActivePlayerShots are usable in play. The original code
     * declared an array of three and then initialised ten entries of it,
     * writing seven elements past the end of the stack buffer.
     */
    constexpr int MaxPlayerShots = 10;
    /// Projectiles the player may keep in flight at once.
    constexpr int ActivePlayerShots = 3;

    /// Upper bound for the enemy fleet and its projectile pool.
    constexpr int MaxEnemies = 100;

    /// Parallax layers in the background starfield.
    constexpr int StarLayers = 3;
    /// Stars per parallax layer.
    constexpr int StarsPerLayer = 100;
}
