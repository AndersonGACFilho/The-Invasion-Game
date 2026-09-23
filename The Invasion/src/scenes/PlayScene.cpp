#include "scenes/PlayScene.h"
#include "platform/ResourceLibrary.h"
#include "platform/AudioDirector.h"
#include "systems/Starfield.h"
#include "systems/CombatResolver.h"
#include <allegro5/allegro.h>

void PlayScene::onEnter()
{
    m_player.reset();
    m_fleet.reset();
    m_playerWeapon.reset();
    m_enemyWeapon.reset();
    m_context.starfield.reset();

    for (bool& key : m_keys)
        key = false;

    // The original started this counter at nine, outside the zero-to-eight
    // range of its drawing switch, leaving the ship invisible for the first
    // twenty frames of every match.
    m_frame = 0;
    m_frameCount = 0;

    m_context.audio.playBattleMusic();
}

void PlayScene::onExit()
{
    m_context.audio.stopBattleMusic();
}

void PlayScene::handleEvent(const ALLEGRO_EVENT& event)
{
    if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_W: m_keys[Up] = true; break;
        case ALLEGRO_KEY_S: m_keys[Down] = true; break;
        case ALLEGRO_KEY_A: m_keys[Left] = true; break;
        case ALLEGRO_KEY_D: m_keys[Right] = true; break;
        case ALLEGRO_KEY_SPACE: m_playerWeapon.fire(m_player); break;
        case ALLEGRO_KEY_ESCAPE: transitionTo(SceneId::Menu); break;
        default: break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_W: m_keys[Up] = false; break;
        case ALLEGRO_KEY_S: m_keys[Down] = false; break;
        case ALLEGRO_KEY_A: m_keys[Left] = false; break;
        case ALLEGRO_KEY_D: m_keys[Right] = false; break;
        default: break;
        }
    }
}

void PlayScene::applyMovement()
{
    // Original priority: one direction per tick, in this order.
    if (m_keys[Up])
        m_player.moveUp();
    else if (m_keys[Down])
        m_player.moveDown();
    else if (m_keys[Left])
        m_player.moveLeft();
    else if (m_keys[Right])
        m_player.moveRight();
}

void PlayScene::advanceAnimation()
{
    m_frameCount++;
    if (m_frameCount < FrameDelay)
        return;

    m_frameCount = 0;
    m_frame++;
    if (m_frame >= ResourceLibrary::PlayerFrames)
        m_frame = 0;
}

void PlayScene::update()
{
    advanceAnimation();

    m_context.starfield.update();
    applyMovement();

    m_fleet.spawn(m_player);
    m_fleet.update();
    m_enemyWeapon.fire(m_player, m_fleet);
    m_playerWeapon.update(m_player, m_fleet);
    m_enemyWeapon.update(m_player, m_fleet);
    CombatResolver::resolveCrossfire(m_playerWeapon, m_enemyWeapon, m_player, m_fleet);

    if (m_player.isDead())
        transitionTo(SceneId::GameOver);
}

void PlayScene::draw()
{
    const ResourceLibrary& resources = m_context.resources;

    m_context.starfield.draw();
    m_fleet.draw(resources);
    al_draw_bitmap(resources.playerFrame(m_frame),
                   m_player.x() - 32, m_player.y() - 32, 0);
    m_playerWeapon.draw(m_player);
    m_enemyWeapon.draw(m_fleet);
    m_hud.draw(m_player, m_playerWeapon, m_fleet, resources);
}
