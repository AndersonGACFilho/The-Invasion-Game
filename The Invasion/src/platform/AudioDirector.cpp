#include "platform/AudioDirector.h"
#include "platform/ResourceLibrary.h"

AudioDirector::~AudioDirector()
{
    al_destroy_sample_instance(m_menu);
    al_destroy_sample_instance(m_battle);
    al_destroy_sample_instance(m_beep);
}

bool AudioDirector::attach(const ResourceLibrary& resources)
{
    m_battle = al_create_sample_instance(resources.battleMusic());
    m_menu = al_create_sample_instance(resources.menuMusic());
    m_beep = al_create_sample_instance(resources.beep());

    if (!m_battle || !m_menu || !m_beep)
        return false;

    ALLEGRO_MIXER* mixer = al_get_default_mixer();
    al_attach_sample_instance_to_mixer(m_battle, mixer);
    al_attach_sample_instance_to_mixer(m_menu, mixer);
    al_attach_sample_instance_to_mixer(m_beep, mixer);

    al_set_sample_instance_playmode(m_battle, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(m_menu, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(m_beep, ALLEGRO_PLAYMODE_ONCE);

    // Original mix: the tracks sit far below the interface beep.
    al_set_sample_instance_gain(m_menu, 0.01f);
    al_set_sample_instance_gain(m_battle, 0.008f);
    al_set_sample_instance_gain(m_beep, 1.0f);

    return true;
}

void AudioDirector::playMenuMusic() { al_play_sample_instance(m_menu); }
void AudioDirector::stopMenuMusic() { al_stop_sample_instance(m_menu); }
void AudioDirector::playBattleMusic() { al_play_sample_instance(m_battle); }
void AudioDirector::stopBattleMusic() { al_stop_sample_instance(m_battle); }
void AudioDirector::playBeep() { al_play_sample_instance(m_beep); }
