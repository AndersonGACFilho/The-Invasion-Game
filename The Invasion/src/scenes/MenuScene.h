#pragma once
#include "scenes/Scene.h"
#include "core/GameContext.h"

/**
 * @brief The main menu: play, credits or quit.
 */
class MenuScene : public Scene
{
public:
    explicit MenuScene(GameContext context) : m_context(context) {}

    void onEnter() override;
    void onExit() override;
    void handleEvent(const ALLEGRO_EVENT& event) override;
    void update() override;
    void draw() override;

private:
    /// Acts on the highlighted entry.
    void confirm();

    GameContext m_context;

    /// 1 = play, 2 = credits, 3 = quit. Wraps at both ends.
    int m_selection = 1;
};
