#pragma once
#include "scenes/Scene.h"
#include "core/GameContext.h"
#include "entities/Player.h"
#include "systems/EnemyFleet.h"
#include "systems/PlayerWeapon.h"
#include "systems/EnemyWeapon.h"
#include "systems/Hud.h"

/**
 * @brief The match itself: input, simulation and frame composition.
 */
class PlayScene : public Scene
{
public:
    explicit PlayScene(GameContext context) : m_context(context) {}

    void onEnter() override;
    void onExit() override;
    void handleEvent(const ALLEGRO_EVENT& event) override;
    void update() override;
    void draw() override;

    /// Score reached when the match ended, read by @ref GameOverScene.
    int finalScore() const { return m_player.score(); }

private:
    enum Direction { Up, Down, Left, Right, DirectionCount };

    void applyMovement();
    void advanceAnimation();

    GameContext m_context;
    Player m_player;
    EnemyFleet m_fleet;
    PlayerWeapon m_playerWeapon;
    EnemyWeapon m_enemyWeapon;
    Hud m_hud;

    bool m_keys[DirectionCount] = { false, false, false, false };

    short int m_frame = 0;
    short int m_frameCount = 0;

    /// Clock ticks between ship animation frames.
    static constexpr short int FrameDelay = 20;
};
