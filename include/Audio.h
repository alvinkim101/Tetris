#pragma once

struct Mix_Chunk;
struct _Mix_Music;

namespace Tetris
{

class Audio
{
    public:
        enum class Sound
        {
            #define SOUND_EFFECT(name) name,
            #include "macro/Sound.h"
        };

        bool Load();
        void Free();

        // TODO: Figure out the reason for audio delay/stutter/cut out (is it because I'm on WSL?)
        void PlayMusic() const;
        void PlaySound(Sound sound) const;

    private:
        #define SOUND_EFFECT(name) Mix_Chunk* m_##name##Sound = nullptr;
        #include "macro/Sound.h"

        _Mix_Music* m_bgm = nullptr;
};

}
