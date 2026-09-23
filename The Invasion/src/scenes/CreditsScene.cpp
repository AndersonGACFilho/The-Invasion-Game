#include "scenes/CreditsScene.h"
#include "platform/ResourceLibrary.h"
#include "platform/AudioDirector.h"
#include "systems/Starfield.h"
#include <allegro5/allegro_font.h>

namespace {
constexpr int BlinkOn = 30;
constexpr int BlinkPeriod = 60;
}

void CreditsScene::onEnter()
{
    m_blink = 0;
    m_context.audio.playMenuMusic();
}

void CreditsScene::handleEvent(const ALLEGRO_EVENT& event)
{
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        m_context.audio.playBeep();
        transitionTo(SceneId::Menu);
    }
}

void CreditsScene::update()
{
    m_context.starfield.update();
}

void CreditsScene::draw()
{
    const ResourceLibrary& resources = m_context.resources;

    m_context.starfield.draw();

    al_draw_text(resources.titleFont(), al_map_rgb(20, 255, 200), 640, 25,
                 ALLEGRO_ALIGN_CENTER, "Credits");
    al_draw_text(resources.buttonFont(), al_map_rgb(110, 255, 110), 540, 360,
                 ALLEGRO_ALIGN_CENTER, "PROGRAMMER      ANDERSON GONÇALVES");
    al_draw_text(resources.buttonFont(), al_map_rgb(110, 255, 110), 540, 390,
                 ALLEGRO_ALIGN_CENTER, "GAME DESIGN      ANDERSON GONÇALVES");
    al_draw_text(resources.buttonFont(), al_map_rgb(110, 255, 110), 615, 420,
                 ALLEGRO_ALIGN_CENTER,
                 "ARTIST                    ANDERSON GONÇALVES    HUGO CITY");

    m_blink++;
    if (m_blink > BlinkOn)
        al_draw_text(resources.buttonFont(), al_map_rgb(175, 255, 110), 615, 648,
                     ALLEGRO_ALIGN_CENTER,
                     "PRESS ANY BUTTON FOR RETURN TO MAIN MENU");
    if (m_blink == BlinkPeriod)
        m_blink = 0;
}
