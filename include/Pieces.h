#pragma once

#include <array>
#include <vector>
#include <random>
#include <utility>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// Using single bag randomization
class Pieces
{
    public:
        Pieces();

        const std::pair<glm::uvec3, std::vector<std::vector<glm::ivec2>>>* Random();

    private:
        void Shuffle();

        static const std::vector<std::pair<glm::uvec3, std::vector<std::vector<glm::ivec2>>>> Tetrominoes;

        static thread_local std::default_random_engine m_rng;

        static constexpr uint8_t NumberOfPieces = 7;

        std::array<uint8_t, NumberOfPieces> m_indices;
        uint8_t m_currentIndex = 0;
};
