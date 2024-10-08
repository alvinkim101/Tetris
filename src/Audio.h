#pragma once

class Audio
{
    public:
        bool Load();
        void Free();

        void PlayMusic() const;

    private:
        #define SOUND_EFFECT(name) struct Mix_Chunk* m_##name##Sound;
        #include "Macros/Sound.h"

        struct _Mix_Music* m_bgm;
};
