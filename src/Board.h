#pragma once

#include "Color.h"

#include <array>
#include <tuple>

#include <glm/vec3.hpp>

class Board
{
    using Color = glm::uvec3;

    public:
        Board(class SDL_Window& window, class SDL_Renderer& renderer);
        void Render() const;
        bool IsOccupied(int8_t row, int8_t col) const; // If white then vacant

        // Rows -> Y, Cols -> X
        static constexpr uint8_t Rows = 20;
        static constexpr uint8_t Cols = 10;
        static constexpr uint8_t WidthDivisor = 3; // Used to divide rendering width

    private:
        std::array<std::array<Color, Cols>, Rows> m_board;
        class SDL_Window& m_window;
        class SDL_Renderer& m_renderer;

        static constexpr Color White{RGB_WHITE};
};
