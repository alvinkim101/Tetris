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
    #include "Macros/Sound.h"

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
    #include "Macros/Sound.h"

    Mix_FreeMusic(m_bgm);
}

void Audio::PlayMusic() const
{
    Mix_PlayMusic(m_bgm, -1);
}
