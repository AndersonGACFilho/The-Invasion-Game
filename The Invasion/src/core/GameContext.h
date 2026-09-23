#pragma once

class ResourceLibrary;
class AudioDirector;
class Starfield;

/**
 * @brief The state every scene shares, passed by reference.
 *
 * Replaces the handful of file-scope globals the original relied on.
 */
struct GameContext
{
    ResourceLibrary& resources;
    AudioDirector& audio;
    Starfield& starfield;
    int& highScore;
};
