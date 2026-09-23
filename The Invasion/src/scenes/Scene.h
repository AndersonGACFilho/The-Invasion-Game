#pragma once
#include <allegro5/allegro.h>

/// Identifies each screen, and the two terminal transitions.
enum class SceneId { Menu, Credits, Play, GameOver, Quit, None };

/**
 * @brief Contract for a single screen.
 *
 * Replaces the four nested @c while loops of the original: every screen
 * answers for itself and @ref Game decides only the transitions.
 */
class Scene
{
public:
    virtual ~Scene() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void handleEvent(const ALLEGRO_EVENT& event) = 0;

    /// Advances the simulation. Called once per clock tick.
    virtual void update() = 0;

    virtual void draw() = 0;

    /**
     * @brief Decides whether this screen draws on the current iteration.
     *
     * Menu, credits and game over redraw on every pass, which is what sets
     * the cadence of their blinking prompt. Only the match waits for the
     * event queue to drain.
     */
    virtual bool wantsDraw(bool queueEmpty) const
    {
        (void)queueEmpty;
        return true;
    }

    /// @return The requested next screen, or SceneId::None to stay put.
    SceneId nextScene() const { return m_next; }
    void clearTransition() { m_next = SceneId::None; }

protected:
    void transitionTo(SceneId id) { m_next = id; }

private:
    SceneId m_next = SceneId::None;
};
