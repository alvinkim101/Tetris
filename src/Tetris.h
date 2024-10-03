#pragma once

#include "Board.h"
#include "Tetromino.h"

#include <memory>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_keycode.h>

#define SOUND_EFFECTS \
    X(move) \
    X(rotate) \
    X(land) \
    X(lineClear) \
    X(fourLineClear) \
    X(levelUp) \
    X(gameOver)

class Tetris
{
    public:
        Tetris();
        ~Tetris();

        void RunGame();

    private:
        void Initialize();
        void Deinitialize();

        void Update();
        void Render();

        void ParseInput(SDL_Keycode key);

        std::unique_ptr<Board> m_board;
        std::unique_ptr<Tetromino> m_tetromino;

        SDL_DisplayMode m_displayMode;
        class SDL_Window* m_window;
        class SDL_Renderer* m_renderer;

        struct _Mix_Music* m_backgroundMusic;
        #define X(soundEffect) struct Mix_Chunk* m_##soundEffect##Sound;
        SOUND_EFFECTS
        #undef X
        
        bool m_initialized = false;
        bool m_running;
};
