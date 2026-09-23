#include "platform/HighScoreStore.h"
#include "platform/Paths.h"

#include <fstream>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::string HighScoreStore::path()
{
    return Paths::highScoreFile();
}

void HighScoreStore::init()
{
#ifdef __EMSCRIPTEN__
    EM_ASM(
        FS.mkdir('/save');
        FS.mount(IDBFS, {}, '/save');
        Module.syncdone = 0;
        FS.syncfs(true, function (err) { Module.syncdone = 1; });
    );

    // ASYNCIFY lets us yield to the browser while IndexedDB loads.
    while (emscripten_run_script_int("Module.syncdone") == 0)
        emscripten_sleep(50);
#endif
}

int HighScoreStore::load()
{
    int value = 0;
    std::ifstream in(path().c_str(), std::ios::binary);
    if (in)
        in >> value;
    return value;
}

void HighScoreStore::save(int value)
{
    {
        std::ofstream out(path().c_str(), std::ios::binary);
        out << value;
    }

#ifdef __EMSCRIPTEN__
    EM_ASM(FS.syncfs(false, function (err) {}););
#endif
}
