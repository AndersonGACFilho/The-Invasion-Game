#pragma once
#include "core/Config.h"

/**
 * @brief The three-layer parallax background.
 *
 * Layers scroll at 8, 4 and 2 pixels per tick, so the fastest reads as
 * the nearest.
 */
class Starfield
{
public:
    /// Scatters the stars and assigns their colours and layer speeds.
    void reset();
    /// Scrolls every star, wrapping it round the right edge.
    void update();
    void draw() const;

private:
    struct Star
    {
        short int x = 0;
        short int y = 0;
        short int velocity = 0;
        short int r = 0;
        short int g = 0;
        short int b = 0;
    };

    Star m_stars[Config::StarLayers][Config::StarsPerLayer];
};
