#include "Pieces.h"
#include "macro/Color.h"

#include <algorithm>
#include <iostream>

namespace Tetris
{

// Contains all seven distinct tetrominoes
// First value contains color, second value contains possible orientations
// First coordinates are at origin
// Clockwise ordered
const std::vector<Pieces::Piece> Pieces::Tetrominoes =
{
    /* I */ { glm::uvec3(RGB_CYAN), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(1, 0)}, {glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(0, 1)}}},
    /* O */ { glm::uvec3(RGB_YELLOW), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1) }}},
    /* T */ { glm::uvec3(RGB_PURPLE), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 0), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 0), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, 0) }}},
    /* J */ { glm::uvec3(RGB_BLUE), {{ glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 1), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(-1, -1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, -1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(1, 1) }}},
    /* L */ { glm::uvec3(RGB_ORANGE), {{ glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 1), glm::ivec2(-1, 0), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, -1), glm::ivec2(1, 0) }}},
    /* S */ { glm::uvec3(RGB_GREEN), {{ glm::ivec2(0, 0), glm::ivec2(-1, 1), glm::ivec2(0, 1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(1, 0), glm::ivec2(1, 1) }}},
    /* Z */ { glm::uvec3(RGB_RED), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 1) }, { glm::ivec2(0, 0), glm::ivec2(0, 1), glm::ivec2(1, -1), glm::ivec2(1, 0) }}}
};

thread_local std::default_random_engine Pieces::m_rng;

Pieces::Pieces()
{
    std::random_device rd;
    m_rng.seed(rd());

    for (int i = 0; i < m_indices.size(); i++)
    {
        m_indices[i] = i;
    }

    Shuffle();
}

const Pieces::Piece* Pieces::Random()
{
    if (m_currentIndex == NumberOfPieces)
    {
        Shuffle();
        m_currentIndex = 0;
    }

    return &Tetrominoes[m_indices[m_currentIndex++]];
}

void Pieces::Shuffle()
{
    std::shuffle(m_indices.begin(), m_indices.end(), m_rng);
}

}
