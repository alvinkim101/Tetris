#pragma once

#include "Piece.h"
#include "Random.h"

namespace Tetris
{

class PieceSelector
{
    public:
        PieceSelector();

        // Single bag randomization
        const Piece* Select();

    private:
        void Shuffle();

        // thread_local in case cache coherence comes into play
        static thread_local Random RNG;

        // Stores the shuffled index values of Pieces
        std::vector<uint8_t> m_piecesIndexes;
        uint8_t m_index = 0;
};

}
