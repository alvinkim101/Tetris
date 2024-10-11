#pragma once

#include <array>
#include <vector>
#include <random>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Tetris
{

// Using single bag randomization
class Pieces
{
    public:
        static constexpr size_t Size = 4;

        struct Piece
        {
            Piece(const glm::uvec3& color, const std::vector<std::array<glm::ivec2, Size>>& orientations) : color(color), orientations(orientations)
            {

            }

            std::vector<std::array<glm::ivec2, Size>> orientations;
            glm::uvec3 color;
        };

        Pieces();

        const Piece* Random();

    private:
        void Shuffle();

        static const std::vector<Piece> Tetrominoes;

        static thread_local std::default_random_engine m_rng;

        static constexpr uint8_t NumberOfPieces = 7;

        std::array<uint8_t, NumberOfPieces> m_indices;
        uint8_t m_currentIndex = 0;
};

}
