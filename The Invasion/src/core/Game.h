#pragma once
#include "platform/Platform.h"
#include "platform/ResourceLibrary.h"
#include "platform/AudioDirector.h"
#include "platform/HighScoreStore.h"
#include "systems/Starfield.h"
#include "scenes/Scene.h"
#include "scenes/MenuScene.h"
#include "scenes/CreditsScene.h"
#include "scenes/PlayScene.h"
#include "scenes/GameOverScene.h"

/**
 * @brief Composition root: wires the game up, runs the event loop and
 *        decides the transitions between screens.
 */
class Game
{
public:
    Game();

    /**
     * @brief Boots the platform, loads assets and runs until the player quits.
     * @return @c EXIT_SUCCESS, or @c EXIT_FAILURE if start-up failed.
     */
    int run();

private:
    Scene* sceneFor(SceneId id);
    void switchTo(SceneId id);

    /**
     * @brief Commits the last match's score to the record, if it beat it.
     *
     * Deliberately deferred until the menu is reached, matching the
     * original, where the comparison sat at the end of the outer loop.
     */
    void commitScore();

    Platform m_platform;
    ResourceLibrary m_resources;
    AudioDirector m_audio;
    Starfield m_starfield;
    HighScoreStore m_store;

    int m_highScore = 0;
    int m_lastScore = 0;

    MenuScene m_menu;
    CreditsScene m_credits;
    PlayScene m_play;
    GameOverScene m_gameOver;

    Scene* m_current = nullptr;
};
