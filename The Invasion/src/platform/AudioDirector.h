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
    /// Mix levels.
    ///
    /// The original set the tracks to 0.01 and 0.008 against a beep at
    /// 1.0, leaving the music roughly a hundred times quieter than the
    /// interface click. These levels put the music in front and bring the
    /// click down to sit with it.
    static constexpr float MenuMusicGain = 0.45f;
    static constexpr float BattleMusicGain = 0.40f;
    static constexpr float BeepGain = 0.50f;

    ALLEGRO_SAMPLE_INSTANCE* m_menu = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* m_battle = nullptr;
    ALLEGRO_SAMPLE_INSTANCE* m_beep = nullptr;
};
