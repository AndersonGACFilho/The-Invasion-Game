#pragma once
#include "scenes/Scene.h"
#include "core/GameContext.h"

/**
 * @brief The end-of-match screen. Any key returns to the menu.
 */
class GameOverScene : public Scene
{
public:
    explicit GameOverScene(GameContext context) : m_context(context) {}

    /// Supplied by @ref Game when the match ends.
    void setScore(int score) { m_score = score; }

    void onEnter() override;
    void onExit() override;
    void handleEvent(const ALLEGRO_EVENT& event) override;
    void update() override;
    void draw() override;

private:
    GameContext m_context;
    int m_score = 0;
    int m_blink = 0;
};
