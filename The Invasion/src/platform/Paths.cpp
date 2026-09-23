#include "platform/Paths.h"

#include <cstdlib>

#ifndef _WIN32
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#if defined(THE_INVASION_DATADIR) && !defined(_WIN32)
namespace {

/// Creates a directory and every missing parent, ignoring those present.
void makeDirectories(const std::string& path)
{
    std::string partial;
    for (std::string::size_type i = 1; i <= path.size(); ++i)
    {
        if (i == path.size() || path[i] == '/')
        {
            partial = path.substr(0, i);
            ::mkdir(partial.c_str(), 0755);
        }
    }
}

} // namespace
#endif

bool Paths::isInstalled()
{
#ifdef THE_INVASION_DATADIR
    return true;
#else
    return false;
#endif
}

const char* Paths::dataDir()
{
#ifdef THE_INVASION_DATADIR
    return THE_INVASION_DATADIR;
#else
    return ".";
#endif
}

bool Paths::enterDataDir()
{
#if defined(THE_INVASION_DATADIR) && !defined(_WIN32)
    return ::chdir(THE_INVASION_DATADIR) == 0;
#else
    return true;
#endif
}

std::string Paths::highScoreFile()
{
#ifdef __EMSCRIPTEN__
    return "/save/HighScore";
#elif defined(THE_INVASION_DATADIR) && !defined(_WIN32)
    std::string base;
    if (const char* xdg = std::getenv("XDG_DATA_HOME"); xdg && *xdg)
        base = xdg;
    else if (const char* home = std::getenv("HOME"); home && *home)
        base = std::string(home) + "/.local/share";
    else
        return "HighScore";

    base += "/the-invasion";
    makeDirectories(base);
    return base + "/HighScore";
#else
    return "HighScore";
#endif
}
