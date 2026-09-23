#include "scenes/GameOverScene.h"
#include "platform/ResourceLibrary.h"
#include "platform/AudioDirector.h"
#include "systems/Starfield.h"
#include <allegro5/allegro_font.h>

namespace {
constexpr int BlinkOn = 30;
constexpr int BlinkPeriod = 60;
}

void GameOverScene::onEnter()
{
    m_blink = 0;
    m_context.audio.playMenuMusic();
}

void GameOverScene::onExit()
{
    m_context.audio.stopMenuMusic();
}

void GameOverScene::handleEvent(const ALLEGRO_EVENT& event)
{
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        m_context.audio.playBeep();
        transitionTo(SceneId::Menu);
    }
}

void GameOverScene::update()
{
    m_context.starfield.update();
}

void GameOverScene::draw()
{
    const ResourceLibrary& resources = m_context.resources;

    m_context.starfield.draw();

    // The record is only committed once this screen closes, which is what
    // lets the plain greater-than test announce a fresh record here.
    if (m_score > m_context.highScore)
        al_draw_text(resources.buttonFont(), al_map_rgb(175, 255, 120), 640, 400,
                     ALLEGRO_ALIGN_CENTER, "New Highscore");
    else
        al_draw_textf(resources.buttonFont(), al_map_rgb(175, 255, 120), 640, 400,
                      ALLEGRO_ALIGN_CENTER, "HighScore - %d", m_context.highScore);

    al_draw_text(resources.titleFont(), al_map_rgb(255, 20, 20), 640, 260,
                 ALLEGRO_ALIGN_CENTER, "Game Over");
    al_draw_textf(resources.buttonFont(), al_map_rgb(175, 255, 120), 640, 500,
                  ALLEGRO_ALIGN_CENTER, "Your Score - %d", m_score);

    m_blink++;
    if (m_blink > BlinkOn)
        al_draw_text(resources.buttonFont(), al_map_rgb(175, 255, 110), 615, 648,
                     ALLEGRO_ALIGN_CENTER,
                     "PRESS ANY BUTTON FOR RETURN TO MAIN MENU");
    if (m_blink == BlinkPeriod)
        m_blink = 0;
}
