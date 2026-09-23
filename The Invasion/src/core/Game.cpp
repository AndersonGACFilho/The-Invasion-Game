#include "core/Game.h"
#include "core/Config.h"
#include "platform/Paths.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

namespace {

GameContext makeContext(ResourceLibrary& resources,
                        AudioDirector& audio,
                        Starfield& starfield,
                        int& highScore)
{
    return GameContext{ resources, audio, starfield, highScore };
}

} // namespace

Game::Game()
    : m_menu(makeContext(m_resources, m_audio, m_starfield, m_highScore))
    , m_credits(makeContext(m_resources, m_audio, m_starfield, m_highScore))
    , m_play(makeContext(m_resources, m_audio, m_starfield, m_highScore))
    , m_gameOver(makeContext(m_resources, m_audio, m_starfield, m_highScore))
{
}

Scene* Game::sceneFor(SceneId id)
{
    switch (id)
    {
    case SceneId::Menu: return &m_menu;
    case SceneId::Credits: return &m_credits;
    case SceneId::Play: return &m_play;
    case SceneId::GameOver: return &m_gameOver;
    default: return nullptr;
    }
}

void Game::commitScore()
{
    if (m_lastScore > m_highScore)
    {
        m_highScore = m_lastScore;
        m_store.save(m_highScore);
    }
    m_lastScore = 0;
}

void Game::switchTo(SceneId id)
{
    if (m_current == &m_play)
    {
        m_lastScore = m_play.finalScore();
        m_gameOver.setScore(m_lastScore);
    }

    if (id == SceneId::Menu)
        commitScore();

    m_current->onExit();
    m_current = sceneFor(id);
    m_current->onEnter();
}

int Game::run()
{
    // Installed builds keep their assets under a fixed prefix; the loaders
    // below all use paths relative to the working directory.
    if (!Paths::enterDataDir())
    {
        std::fprintf(stderr, "ERROR: could not enter data directory %s\n",
                     Paths::dataDir());
        return EXIT_FAILURE;
    }

    if (!m_platform.boot(Config::ScreenWidth, Config::ScreenHeight,
                         Config::Fps, "The Invasion"))
        return EXIT_FAILURE;

    if (!m_resources.load())
    {
        std::fprintf(stderr, "ERROR: assets are missing, aborting\n");
        return EXIT_FAILURE;
    }

    if (!m_audio.attach(m_resources))
    {
        std::fprintf(stderr, "ERROR: could not set up audio\n");
        return EXIT_FAILURE;
    }

    m_store.init();
    m_highScore = m_store.load();

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    m_starfield.reset();

    m_platform.startClock();
    m_current = &m_menu;
    m_current->onEnter();

    ALLEGRO_EVENT_QUEUE* queue = m_platform.events();

    while (true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;

        m_current->handleEvent(event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            m_current->update();
            m_current->markDirty();
        }

        if (m_current->wantsDraw(al_is_event_queue_empty(queue)))
        {
            m_platform.beginFrame();
            m_current->draw();
            m_platform.present();
            m_current->clearDirty();
        }

        const SceneId next = m_current->nextScene();
        if (next == SceneId::None)
            continue;

        m_current->clearTransition();

        if (next == SceneId::Quit)
        {
            if (m_current == &m_play)
                m_lastScore = m_play.finalScore();
            commitScore();
            break;
        }

        switchTo(next);
    }

    m_store.save(m_highScore);
    return EXIT_SUCCESS;
}
