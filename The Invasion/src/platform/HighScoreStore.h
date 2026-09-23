#pragma once

/**
 * @brief Persists the all-time best score.
 *
 * On the desktop this is a plain file next to the executable. In the
 * browser the program never really terminates -- the player closes the
 * tab and any code after the main loop never runs -- so the value is
 * written the moment it changes and lives in an IDBFS mount, which
 * survives across sessions.
 */
class HighScoreStore
{
public:
    /// Mounts and synchronises the backing store. Must precede @ref load.
    void init();

    /// @return The stored score, or zero when nothing has been saved yet.
    int load();

    /// Writes the value and flushes it to persistent storage.
    void save(int value);

private:
    static const char* path();
};
