#include "platform/HighScoreStore.h"
#include <fstream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

const char* HighScoreStore::path()
{
#ifdef __EMSCRIPTEN__
    return "/save/HighScore";
#else
    return "HighScore";
#endif
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
    std::ifstream in(path(), std::ios::binary);
    if (in)
        in >> value;
    return value;
}

void HighScoreStore::save(int value)
{
    {
        std::ofstream out(path(), std::ios::binary);
        out << value;
    }

#ifdef __EMSCRIPTEN__
    EM_ASM(FS.syncfs(false, function (err) {}););
#endif
}
