#include "scenes/MenuScene.h"
#include "platform/ResourceLibrary.h"
#include "platform/AudioDirector.h"
#include "systems/Starfield.h"
#include "core/Config.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

namespace {
constexpr int Highlighted = 100;
constexpr int Dimmed = 75;
}

void MenuScene::onEnter()
{
    m_context.audio.playMenuMusic();
}

void MenuScene::onExit()
{
    m_context.audio.stopMenuMusic();
}

void MenuScene::confirm()
{
    m_context.audio.playBeep();
    switch (m_selection)
    {
    case 1: transitionTo(SceneId::Play); break;
    case 2: transitionTo(SceneId::Credits); break;
    case 3: transitionTo(SceneId::Quit); break;
    default: break;
    }
}

void MenuScene::handleEvent(const ALLEGRO_EVENT& event)
{
    if (event.type != ALLEGRO_EVENT_KEY_DOWN)
        return;

    switch (event.keyboard.keycode)
    {
    case ALLEGRO_KEY_W:
    case ALLEGRO_KEY_UP:
        m_context.audio.playBeep();
        m_selection--;
        break;
    case ALLEGRO_KEY_S:
    case ALLEGRO_KEY_DOWN:
        m_context.audio.playBeep();
        m_selection++;
        break;
    case ALLEGRO_KEY_SPACE:
    case ALLEGRO_KEY_ENTER:
        confirm();
        break;
    default:
        break;
    }

    if (m_selection > 3) m_selection = 1;
    if (m_selection < 1) m_selection = 3;
}

void MenuScene::update()
{
    m_context.starfield.update();
}

void MenuScene::draw()
{
    const ResourceLibrary& resources = m_context.resources;

    // Draw order preserved from the original: the title goes down before
    // the starfield, so the stars twinkle across the lettering.
    al_draw_text(resources.titleFont(), al_map_rgb(0, 255, 100),
                 Config::ScreenWidth - 100, 50, ALLEGRO_ALIGN_RIGHT,
                 "The Invasion");

    m_context.starfield.draw();

    const int shade[3] = {
        m_selection == 1 ? Highlighted : Dimmed,
        m_selection == 2 ? Highlighted : Dimmed,
        m_selection == 3 ? Highlighted : Dimmed,
    };

    al_draw_filled_rectangle(100, 300, 450, 350, al_map_rgb(shade[0], shade[0], shade[0]));
    al_draw_filled_rectangle(100, 400, 450, 450, al_map_rgb(shade[1], shade[1], shade[1]));
    al_draw_filled_rectangle(100, 500, 450, 550, al_map_rgb(shade[2], shade[2], shade[2]));

    al_draw_textf(resources.buttonFont(), al_map_rgb(0, 255, 200), 150, 212,
                  ALLEGRO_ALIGN_LEFT, "HIGHSCORE - %d", m_context.highScore);
    al_draw_text(resources.buttonFont(), al_map_rgb(0, 0, 0), 300, 312,
                 ALLEGRO_ALIGN_RIGHT, "PLAY");
    al_draw_text(resources.buttonFont(), al_map_rgb(0, 0, 0), 325, 412,
                 ALLEGRO_ALIGN_RIGHT, "CREDITS");
    al_draw_text(resources.buttonFont(), al_map_rgb(0, 0, 0), 300, 512,
                 ALLEGRO_ALIGN_RIGHT, "QUIT");
}
