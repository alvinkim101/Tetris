#pragma once

#include "Board.h"
#include "Tetromino.h"
#include "Audio.h"

#include <memory>

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_keycode.h>

struct SDL_Window;
struct SDL_Renderer;

namespace Tetris
{

class Tetris
{
    public:
        // TODO: explicitly state whether classes are copyable / movable
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
        std::unique_ptr<Audio> m_audio;

        SDL_DisplayMode m_displayMode;
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        
        bool m_initialized = false;
        bool m_running;
};

}
