#pragma once

#include "Color.h"

#include <array>
#include <set>
#include <span>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Board
{
    using Color = glm::uvec3;

    public:
        Board(class SDL_Window& window, class SDL_Renderer& renderer);

        void Render() const;

        void Insert(std::span<glm::ivec2> coordinates, const glm::uvec3& color);
        void LineClear();

        bool IsOccupied(int8_t row, int8_t col) const; // If white then vacant

        // Rows -> Y, Cols -> X
        static constexpr uint8_t Rows = 20;
        static constexpr uint8_t Cols = 10;
        static constexpr uint8_t WidthDivisor = 3; // Used to divide rendering width

    private:
        // Be careful of index out of bounds
        void MoveRow(uint8_t srcRow, uint8_t dstRow);
        void CopyRow(uint8_t srcRow, uint8_t dstRow);
        void ClearRow(uint8_t row);

        bool IsRowFull(uint8_t row) const;
        bool IsRowEmpty(uint8_t row) const;

        std::array<std::array<Color, Cols>, Rows> m_board;
        class SDL_Window& m_window;
        class SDL_Renderer& m_renderer;

        std::set<uint8_t> m_recentRows; // Keep track of which rows were most recently inserted into, to optimize line clears. Using std::set for ordered and uniqueness property

        static constexpr Color White{RGB_WHITE};
};
