#pragma once

#include "Color.h"

#include <array>
#include <tuple>

#include <SDL2/SDL_stdinc.h>

class Board
{
    public:
        Board(class SDL_Window* window, class SDL_Renderer* renderer);
        void RenderBoard() const;

    private:
        struct Data
        {
            std::tuple<Uint8, Uint8, Uint8> rgb{RGB_WHITE};
            bool occupied = false;
        };

        static constexpr int rows = 20;
        static constexpr int cols = 10;

        std::array<std::array<Data, cols>, rows> m_board;
        class SDL_Window* m_window;
        class SDL_Renderer* m_renderer;
};
