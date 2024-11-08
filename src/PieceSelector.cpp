#include "PieceSelector.h"
#include "macro/Color.h"

#include <algorithm>
#include <numeric>

namespace Tetris
{

namespace
{

// Contains all seven distinct tetrominoes
// First value contains color, second value contains possible orientations
// First coordinates are at origin
// Orientations are clockwise ordered
const std::vector<Piece> Pieces =
{
    /* I */ { glm::uvec3(RGB_CYAN), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(1, 0)}, {glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(0, 1)}}},
    /* O */ { glm::uvec3(RGB_YELLOW), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1) }}},
    /* T */ { glm::uvec3(RGB_PURPLE), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 0), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 0), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, 0) }}},
    /* J */ { glm::uvec3(RGB_BLUE), {{ glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 1), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(-1, -1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, -1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(1, 1) }}},
    /* L */ { glm::uvec3(RGB_ORANGE), {{ glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 1), glm::ivec2(-1, 0), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, -1), glm::ivec2(1, 0) }}},
    /* S */ { glm::uvec3(RGB_GREEN), {{ glm::ivec2(0, 0), glm::ivec2(-1, 1), glm::ivec2(0, 1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(1, 0), glm::ivec2(1, 1) }}},
    /* Z */ { glm::uvec3(RGB_RED), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 1) }, { glm::ivec2(0, 0), glm::ivec2(0, 1), glm::ivec2(1, -1), glm::ivec2(1, 0) }}}
};

}

thread_local Random PieceSelector::RNG;

PieceSelector::PieceSelector() : m_piecesIndexes(Pieces.size())
{
    std::iota(m_piecesIndexes.begin(), m_piecesIndexes.end(), 0);

    Shuffle();
}

const Piece* PieceSelector::Select()
{
    if (m_index == Pieces.size())
    {
        Shuffle();
        m_index = 0;
    }

    return &Pieces[m_piecesIndexes[m_index++]];
}

void PieceSelector::Shuffle()
{
    std::shuffle(m_piecesIndexes.begin(), m_piecesIndexes.end(), RNG());
}

}
