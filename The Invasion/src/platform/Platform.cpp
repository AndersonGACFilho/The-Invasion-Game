#include "platform/Platform.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <cstdio>

Platform::~Platform()
{
    al_destroy_timer(m_timer);
    al_destroy_event_queue(m_queue);
    al_destroy_display(m_display);
}

bool Platform::boot(int width, int height, int fps, const char* title)
{
    if (!al_init())
    {
        std::fprintf(stderr, "ERROR: could not initialise Allegro\n");
        return false;
    }

    m_display = al_create_display(width, height);
    if (!m_display)
    {
        std::fprintf(stderr, "ERROR: could not create the display\n");
        return false;
    }
    al_set_window_title(m_display, title);

    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_reserve_samples(10);
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    m_timer = al_create_timer(1.0 / fps);
    m_queue = al_create_event_queue();
    if (!m_timer || !m_queue)
    {
        std::fprintf(stderr, "ERROR: could not create the timer or event queue\n");
        return false;
    }

    al_register_event_source(m_queue, al_get_keyboard_event_source());
    al_register_event_source(m_queue, al_get_mouse_event_source());
    al_register_event_source(m_queue, al_get_display_event_source(m_display));
    al_register_event_source(m_queue, al_get_timer_event_source(m_timer));

    al_set_system_mouse_cursor(m_display, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_hide_mouse_cursor(m_display);

    return true;
}

void Platform::beginFrame()
{
    al_clear_to_color(al_map_rgb(2, 2, 10));
}

void Platform::present()
{
    al_flip_display();
}
