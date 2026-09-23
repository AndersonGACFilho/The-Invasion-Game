#pragma once
#include "scenes/Scene.h"
#include "core/GameContext.h"

/**
 * @brief The credits screen. Any key returns to the menu.
 */
class CreditsScene : public Scene
{
public:
    explicit CreditsScene(GameContext context) : m_context(context) {}

    void onEnter() override;
    void handleEvent(const ALLEGRO_EVENT& event) override;
    void update() override;
    void draw() override;

private:
    GameContext m_context;

    /// Frame counter driving the blinking prompt.
    int m_blink = 0;
};
