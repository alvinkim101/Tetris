#pragma once

#include <array>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Tetris
{

struct Piece
{
    static constexpr size_t Size = 4;

    Piece(const glm::uvec3& color, const std::vector<std::array<glm::ivec2, Size>>& orientations) : color(color), orientations(orientations)
    {

    }

    std::vector<std::array<glm::ivec2, Size>> orientations;
    glm::uvec3 color;
};

}
