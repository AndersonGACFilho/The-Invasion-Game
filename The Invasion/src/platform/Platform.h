#pragma once
#include <allegro5/allegro.h>

/**
 * @brief Owns the Allegro lifecycle: addons, window, event queue and clock.
 *
 * No game rule lives here.
 *
 * @note The original created two further timers, @c exposion_time and
 *       @c moving_timer, which were never started, never registered with
 *       the event queue and never destroyed. They are gone.
 */
class Platform
{
public:
    Platform() = default;
    ~Platform();

    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;

    /**
     * @brief Brings up Allegro and opens the window.
     * @return @c false if any stage failed; the caller should abort.
     */
    bool boot(int width, int height, int fps, const char* title);

    ALLEGRO_EVENT_QUEUE* events() const { return m_queue; }
    ALLEGRO_DISPLAY* display() const { return m_display; }

    void startClock() { al_start_timer(m_timer); }

    /// Clears the drawing buffer. Must precede the frame's drawing.
    void beginFrame();

    /**
     * @brief Presents the finished frame.
     *
     * Clearing happens in @ref beginFrame rather than here. WebGL has no
     * true double buffer: the browser composites whatever the drawing
     * buffer holds at its next repaint, so clearing straight after the
     * flip -- as the original did -- presents an empty frame.
     */
    void present();

private:
    ALLEGRO_DISPLAY* m_display = nullptr;
    ALLEGRO_EVENT_QUEUE* m_queue = nullptr;
    ALLEGRO_TIMER* m_timer = nullptr;
};
