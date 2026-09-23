#include "systems/Starfield.h"
#include <allegro5/allegro_primitives.h>
#include <cstdlib>

void Starfield::reset()
{
    const short int speeds[Config::StarLayers] = { 8, 4, 2 };

    for (int layer = 0; layer < Config::StarLayers; ++layer)
    {
        for (int i = 0; i < Config::StarsPerLayer; ++i)
        {
            Star& star = m_stars[layer][i];
            star.velocity = speeds[layer];
            star.x = 5 + rand() % (Config::ScreenWidth - 10);
            star.y = 5 + rand() % (Config::ScreenHeight - 10);
            star.r = rand() % 255;
            star.g = rand() % 255;
            star.b = rand() % 255;
        }
    }
}

void Starfield::update()
{
    for (int layer = 0; layer < Config::StarLayers; ++layer)
    {
        for (int i = 0; i < Config::StarsPerLayer; ++i)
        {
            Star& star = m_stars[layer][i];
            star.x -= star.velocity;
            if (star.x < 0)
                star.x = Config::ScreenWidth;
        }
    }
}

void Starfield::draw() const
{
    for (int layer = 0; layer < Config::StarLayers; ++layer)
    {
        for (int i = 0; i < Config::StarsPerLayer; ++i)
        {
            const Star& star = m_stars[layer][i];
            al_draw_filled_rectangle(star.x, star.y, star.x + 2, star.y + 2,
                                     al_map_rgb(star.r, star.g, star.b));
        }
    }
}
