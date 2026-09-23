#pragma once
#include <string>

/**
 * @brief Resolves where the game reads its assets and writes its state.
 *
 * Assets are loaded through paths relative to the working directory, which
 * is fine for a build run from the source tree but not for a packaged
 * install, where the data lives under a read-only prefix. Defining
 * @c THE_INVASION_DATADIR at compile time switches the game into installed
 * mode: it changes to that directory on start-up and moves its save file
 * into the user's XDG data directory.
 */
namespace Paths
{
    /// @return @c true when the game was built for a system-wide install.
    bool isInstalled();

    /// Directory holding @c Assets/, @c fonts/ and @c sounds/.
    const char* dataDir();

    /**
     * @brief Path of the high score file.
     *
     * Installed builds use @c $XDG_DATA_HOME/the-invasion/HighScore, since
     * the data directory is not writable. Source-tree and browser builds
     * keep their own conventions.
     */
    std::string highScoreFile();

    /// Changes to @ref dataDir. No-op when not an installed build.
    bool enterDataDir();
}
