#pragma once
#include <allegro5/allegro_audio.h>

class ResourceLibrary;

/**
 * @brief Plays and stops the music tracks and the interface beep.
 *
 * Owns the sample instances, including gain and loop mode.
 *
 * @note Under Emscripten the browser keeps the audio context suspended
 *       until the first user gesture, so the menu track only becomes
 *       audible after the first key press. That is browser policy, not a
 *       fault in this class.
 */
class AudioDirector
{
public:
    AudioDirector() = default;
    ~AudioDirector();

    AudioDirector(const AudioDirector&) = delete;
    AudioDirector& operator=(const AudioDirector&) = delete;

    /**
     * @brief Builds the sample instances from the loaded samples.
     * @return @c false if any instance could not be created.
     */
    bool attach(const ResourceLibrary& resources);

    void playMenuMusic();
    void stopMenuMusic();
    void playBattleMusic();
    void stopBattleMusic();
    void playBeep();

private:
    ALLEGRO_SAMPLE_INSTANCE* m_menu = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* m_battle = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* m_beep = nullptr;
};
