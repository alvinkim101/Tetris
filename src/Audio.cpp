#include "Audio.h"

#include <SDL2/SDL_mixer.h>

bool Audio::Load()
{
    #define SOUND_EFFECT(name) \
    m_##name##Sound = Mix_LoadWAV("../assets/"#name".wav"); \
    if (m_##name##Sound == nullptr) \
    { \
        return false; \
    }
    #include "macro/Sound.h"

    m_bgm = Mix_LoadMUS("../assets/A-Type.ogg");
    if (m_bgm == nullptr)
    {
        return false;
    }

    return true;
}

void Audio::Free()
{
    #define SOUND_EFFECT(name) Mix_FreeChunk(m_##name##Sound);
    #include "macro/Sound.h"

    Mix_FreeMusic(m_bgm);
}

void Audio::PlayMusic() const
{
    Mix_PlayMusic(m_bgm, -1);
}

void Audio::PlaySound(Sound sound) const
{
    switch (sound)
    {
        #define SOUND_EFFECT(name) \
        case Sound::name: \
            Mix_PlayChannel(-1, m_##name##Sound, 0); \
            break;
        #include "macro/Sound.h"
    }
}
