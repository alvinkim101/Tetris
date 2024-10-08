#pragma once

class Audio
{
    public:
        enum class Sound
        {
            #define SOUND_EFFECT(name) name,
            #include "Macros/Sound.h"
        };

        bool Load();
        void Free();

        // TODO: Figure out the reason for audio delay/stutter/cut out (is it because I'm on WSL?)
        void PlayMusic() const;
        void PlaySound(Sound sound) const;

    private:
        #define SOUND_EFFECT(name) struct Mix_Chunk* m_##name##Sound;
        #include "Macros/Sound.h"

        struct _Mix_Music* m_bgm;
};
