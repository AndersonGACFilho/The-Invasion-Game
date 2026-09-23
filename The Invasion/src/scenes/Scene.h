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
     * A screen draws once per clock tick, and only once the input backlog
     * has drained. The original redrew the menu, credits and game over on
     * every pass of their loops; on a WebGL canvas that issues several
     * buffer swaps per browser repaint and the picture flickers.
     */
    virtual bool wantsDraw(bool queueEmpty) const
    {
        return queueEmpty && m_dirty;
    }

    /// Marks the screen as needing a redraw. Called on each clock tick.
    void markDirty() { m_dirty = true; }
    void clearDirty() { m_dirty = false; }

    /// @return The requested next screen, or SceneId::None to stay put.
    SceneId nextScene() const { return m_next; }
    void clearTransition() { m_next = SceneId::None; }

protected:
    void transitionTo(SceneId id) { m_next = id; }

private:
    SceneId m_next = SceneId::None;
    bool m_dirty = true;
};
